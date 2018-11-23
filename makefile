# Library Template
#	by:  V. H.

CC=gcc
CFLAGS=-fPIE
ARFLAGS=rcs
targ=$(shell basename $$PWD)
DEPS=$(targ).h
TEST=test
BINR=bin.c
LIB=/data/data/com.termux/files/usr/lib
INC=$(LIB)/../include
BIN=$(LIB)/../bin

default: all
all: $(targ).o lib$(targ).so lib$(targ).a $(TEST).o fix install pack #$(targ)
.PHONY: clean install fix firstinst all default pack

$(targ).o: $(targ).c $(DEPS)
	$(CC) $(CFLAGS) -c $(targ).c -o $@

lib$(targ).so: $(targ).o
	$(CC) $(CFLAGS) -shared -o $@ $^

lib$(targ).a: $(targ).o
	$(AR) $(ARFLAGS) $@ $^

$(TEST).o: $(TEST).c $(targ).o
	$(CC) $(CFLAGS) $^ -o $@

#$(targ): $(BINR) $(targ).o
#	$(CC) $(CFLAGS) $(BINR) -o $@ -L./ -l$(targ)


install: lib$(targ).so
	install -m 755 lib$(targ).so $(LIB)
	install -m 755 $(targ).h $(INC)
	#install -m 755 $(targ) $(BIN)

clean: *.o
	rm *.o

fix: FORCE
	chmod 775 *

firstinst: FORCE
	$(shell export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:`pwd`)

pack: *
	$(shell tar cz -f ../$(targ).tgz ../$(targ))

FORCE:
