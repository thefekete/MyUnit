P=myunit_tests
OBJECTS=  # tested files go here
CC=gcc -std=gnu99 -D_GNU_SOURCE  # gcc with extras (asprintf, etc)
CFLAGS=-Wall -Wno-unused  # all, but don't bitch about commented tests

$(P): $(OBJECTS)

.PHONY: check
check: $(P)
	./$(P)

.PHONY: clean
clean:
	$(RM) $(P) $(OBJECTS)
