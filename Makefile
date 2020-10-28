test: dist/test.exe
	# running test #
	./$<

dist/EventEmitter.o: src/lib/EventEmitter.c
	gcc -c -o $@ $<

dist/test.exe: src/test.c dist/EventEmitter.o
	# compiling $@ #
	gcc -o $@ $^

clean:
	rm -fv dist/*
