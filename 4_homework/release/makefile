all: calc


calc: main.cpp
	g++ main.cpp -o calc


test: calc
	@g++ test.cpp -o test
	@./calc input.txt -o outtmp.txt
	@echo ------------------------
	@./test
	@echo ------------------------
	@rm outtmp.txt
	@rm test


