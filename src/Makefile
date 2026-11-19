CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

TEST_LIB = ./tests/*.c
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
LINK_FLAGS = -lcheck

FILES = arithmetic/*.c compare/*.c converter/*.c other/*.c supplementary/*.c
OBJECTS = *.o 

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux) 
	LINK_FLAGS += -lm -lsubunit
endif

all: my_decimal.a

clean:
	rm -fr *.o *.a *.gch *.gcno *.gcda test gcov_test my_test.info ./report test

my_decimal.a:
	$(CC) $(CFLAGS) -c $(FILES)
	ar rcs my_decimal.a $(OBJECTS)
	ranlib my_decimal.a

gcov_report: 
	$(CC) $(CFLAGS) $(GCOV_FLAGS) ${TEST_LIB} ${FILES} ${LINK_FLAGS} -o test
	./test
	lcov -t "stest" -o my_test.info -c -d .
	genhtml -o report my_test.info
	open report/index.html

test: clean
	$(CC) $(CFLAGS) $(FILES) ${TEST_LIB} ${LINK_FLAGS} -o test
	./test

leaks: test
ifeq ($(UNAME_S), Linux) 
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./test
endif
ifeq ($(UNAME_S), Darwin) 
	leaks -quiet -atExit -- ./test
endif