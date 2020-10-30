test: dist/test.exe
	# running test #
	./$<

dist/Emitter.o: src/lib/Emitter.c
	gcc -c -o $@ $<

dist/test.exe: src/test.c dist/Emitter.o
	# compiling $@ #
	gcc -o $@ $^ -lcstructures

clean:
	rm -fv dist/*
