TARGET=libbt.a
FLAGS=-Wall -Werror -std=c++11
LIBS=-lm -llua -ldl
DIRS=-I../lua/made/include/ -L../lua/made/lib/

all: $(TARGET)

clean:
	rm $(TARGET)
	rm *.o

rebuild: clean all

$(TARGET): *.c *.cpp *.h
	g++ -c *.c *.cpp $(FLAGS) $(DIRS) $(LIBS)
	ar r $(TARGET) *.o
