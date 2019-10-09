TARGET = executable
TEST = test
CC=g++
CFLAGS=-I.
DEPS = ecs.h components.h
TARGET_OBJS = ecs.o components.o main.o
TEST_OBJS = ecs.o components.o test.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

${TARGET}: $(TARGET_OBJS)
	$(CC) -g -o $@ $^ $(CFLAGS)

${TEST}: $(TEST_OBJS)
	$(CC) -g -o $@ $^ $(CFLAGS)

clean:

ifeq ($(OS),Windows_NT)
	del *.o
	del ${TARGET}.exe
	del ${TEST}.exe
else	
	rm -f *.o
	rm -f $(TARGET)
	rm -f $(TEST)
endif

