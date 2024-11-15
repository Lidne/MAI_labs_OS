build: clean
	mkdir build
	gcc main.c -o ./build/main

trace: build
	cd build/; strace ./main 10 0 10 0 10000 100 > ./strace_output.txt 2>&1

run:
	cd build/; ./main $(filter-out $@,$(MAKECMDGOALS))

clean:
	rm -rf ./build/

%:
	@echo ""

0:
	@echo ""