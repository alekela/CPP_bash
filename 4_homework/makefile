all: calc


calc: main.cpp
	g++ main.cpp -o calc

clean: 
	rm -rf *.o

test: calc
	@g++ test.cpp -o test
	@./calc input.txt -o test.txt
	@echo ------------------------
	@./test
	@echo ------------------------
	@rm test.txt
	@rm test


