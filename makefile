OBJS = build/hunch.o build/formula.o build/parser.o

CC = clang++
CFLAGS = -Wall -c -g -I include -std=c++11
LFLAGS = -Wall -g

bin/hunch : $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(LFLAGS) $(OBJS) -o bin/hunch

build/hunch.o : src/hunch.cpp include/hunch.h include/parser.h include/formula.h 
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/hunch.cpp -o build/hunch.o

build/formula.o : src/formula.cpp include/formula.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/formula.cpp -o build/formula.o

build/parser.o : src/parser.cpp include/parser.h include/formula.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/parser.cpp -o build/parser.o

clean:
	$(RM) -r build bin
