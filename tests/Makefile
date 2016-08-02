all: tests

LIBS= -lbt -lm -llua -ldl
DIRS=-I../src -L../src -I../lua/made/include/ -L../lua/made/lib/
FLAGS=-std=c++11 -D_XOPEN_SOURCE=500

rebuild: clean all

tests: tests.cpp ../src/libbt.a
	g++ $(FLAGS) *.cpp catch.hpp -o $@ $(DIRS) $(LIBS)
	./$@ ||: ./$@.exe
	rm $@ ||: rm $@.exe
