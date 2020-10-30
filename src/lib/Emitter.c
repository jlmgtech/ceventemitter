#include <stdlib.h>
#include <cstructures/HashMap.h>
#include <cstructures/Array.h>
#include "../include/Emitter.h"

Emitter* EmitterCreate() {
    Emitter* this = malloc(sizeof(Emitter));
    this->channels = HashMapCreate(1024);
    this->last_id = 0;
    return this;
}

typedef struct {
    size_t id;
    EventCallback callback;
    bool is_one_time;
} Handler;

Handler* HandlerCreate(size_t id, EventCallback callback, bool is_one_time) {
    Handler* this = malloc(sizeof(Handler));
    this->id = id;
    this->callback = callback;
    this->is_one_time = is_one_time;
    return this;
}

void HandlerDestroy(Handler* this) {
    free(this);
}

// TODO - DRY EmitterOnce and EmitterOn
size_t EmitterOnce(Emitter* this, char* topic, EventCallback callback) {
    Array* channel = HashMapGet(this->channels, topic);
    if (channel == NULL) {
        channel = ArrayCreate(1024);
        HashMapSet(this->channels, topic, channel);
    }
    size_t id = this->last_id++;
    Handler* handler = HandlerCreate(id, callback, true);
    ArrayPush(channel, (void*)handler);
    return id;
}
// TODO - DRY EmitterOnce and EmitterOn
size_t EmitterOn(Emitter* this, char* topic, EventCallback callback) {
    Array* channel = HashMapGet(this->channels, topic);
    if (channel == NULL) {
        channel = ArrayCreate(1024);
        HashMapSet(this->channels, topic, channel);
    }
    size_t id = this->last_id++;
    Handler* handler = HandlerCreate(id, callback, false);
    ArrayPush(channel, (void*)handler);
    return id;
}

void EmitterEmit(Emitter* this, char* topic, void* data) {
    Array* channel = HashMapGet(this->channels, topic);
    if (channel != NULL) {
        for (size_t i = 0; i < channel->count; i++) {
            Handler* handler = (Handler*)ArrayGet(channel, i);
            handler->callback(data);
            if (handler->is_one_time) {
                EmitterRemoveListener(this, topic, handler->id);
            }
        }
    }
}

void EmitterRemoveListener(Emitter* this, char* topic, size_t id) {
    Array* channel = HashMapGet(this->channels, topic);
    if (channel == NULL) {
        return;
    }

    Array* newChannel = ArrayCreate(0);

    for (size_t i = 0; i < channel->count; i++) {
        Handler* handler = (Handler*)ArrayGet(channel, i);
        if (handler->id == id) {
            HandlerDestroy(handler);
        } else {
            ArrayPush(newChannel, (void*)handler);
        }
    }
    ArrayDestroy(channel);
    HashMapSet(this->channels, topic, newChannel);
}

void EmitterDestroy(Emitter* this) {
    for (size_t i = 0; i < this->channels->keys->count; i++) {
        char* key = this->channels->keys->elements[i];
        Array* channel = HashMapGet(this->channels, key);
        for (int j = 0; j < channel->count; j++) {
            Handler* handler = (Handler*)ArrayGet(channel, j);
            HandlerDestroy(handler);
        }
        ArrayDestroy(channel);
    }
    HashMapDestroy(this->channels);
    free(this);
}
