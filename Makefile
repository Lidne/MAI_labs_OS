build: clean
	mkdir build
	gcc parent.c -o ./build/parent.out
	gcc childSpaces.c -o ./build/childSpaces.out
	gcc childUpper.c -o ./build/childUpper.out

trace: build
	strace ./build/main.out > ./build/strace_output.txt 2>&1

run:
	cd build/; ./parent.out

clean:
	rm -rf ./build/