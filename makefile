OBJS = build/hunch.o build/formula.o build/parser.o build/clausifier/hunchClausifier.o \
		 build/clausifier/clausifierFactory.o build/clausifier/clausifier.o 

CC = clang++
CFLAGS = -Wall -c -g -I include -std=c++11 -stdlib=libc++
LFLAGS = -Wall -g -stdlib=libc++

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

build/clausifier/clausifierFactory.o : 	src/clausifier/clausifierFactory.cpp \
										include/clausifier/clausifierFactory.h \
										include/clausifier/clausifier.h \
										include/clausifier/hunchClausifier.h 
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/clausifier/clausifierFactory.cpp -o build/clausifier/clausifierFactory.o

build/clausifier/clausifier.o : src/clausifier/clausifier.cpp \
								include/clausifier/clausifier.h \
								include/formula.h 
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/clausifier/clausifier.cpp -o build/clausifier/clausifier.o

build/clausifier/hunchClausifier.o :	src/clausifier/hunchClausifier.cpp \
										include/clausifier/hunchClausifier.h \
										include/clausifier/clausifier.h \
										include/formula.h 
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/clausifier/hunchClausifier.cpp -o build/clausifier/hunchClausifier.o

clean:
	$(RM) -r build bin
