# Library Template
#	by:  V. H.

CC=gcc
CFLAGS=-fPIE
ARFLAGS=rcs
targ=$(shell basename $$PWD)
DEPS=$(targ).h
LIB=/data/data/com.termux/files/usr/lib
INC=$(LIB)/../include
BIN=$(LIB)/../bin

default: all
all: $(targ).o lib$(targ).so lib$(targ).a test.o fix install
.PHONY: clean install fix firstinst all default

$(targ).o: $(targ).c $(DEPS)
	$(CC) $(CFLAGS) -c $(targ).c -o $@

lib$(targ).so: $(targ).o
	$(CC) $(CFLAGS) -shared -o $@ $^

lib$(targ).a: $(targ).o
	$(AR) $(ARFLAGS) $@ $^

test.o: test.c $(targ).o
	$(CC) $(CFLAGS) $^ -o $@

#$(targ): bin.c $(targ).o
#	$(CC) $(CFLAGS) bin.c -o $@ -L./ -l$(targ)


install: lib$(targ).so
	install -m 755 lib$(targ).so $(LIB)
	install -m 755 $(targ).h $(INC)
	#install -m 755 $(targ) $(BIN)

clean: *.o
	rm *.o

fix: *
	chmod 775 *

firstinst: FORCE
	$(shell export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:`pwd`)

FORCE:
