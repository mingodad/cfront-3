#Makefile for the CC translator
#	BSD 4.2 or later should first run bsd.sed in the scratch directory
#	and set CCFLAGS=-DBSD
#	also set BSD=1

CCFLAGS=-Os -g
scratchCC ?= gcc

BSD=
PATH:=$(CURDIR):$(PATH)

#For first make (bootstrap):
#	make scratch		#on system V, BSD 4.1 or earlier
#Otherwise:
#	make
#

CC	=	CC

.PHONY: demo
all: cfront munch libC.a

libC.a:	always
	cd lib/mk && $(MAKE) CC=$(CC) CCFLAGS="$(CCFLAGS)" BSD=$(BSD)
	mv lib/mk/libC.a .

munch:	_munch/munch.c
	cc -o munch _munch/munch.c

cfront: always
	$(MAKE) -C src

demo: all
	$(MAKE) -C demo/hello

scratch: always
	cd scratch && $(MAKE) CC=$(scratchCC) BSD=$(BSD) CCFLAGS="$(CCFLAGS)"

#This target will populate the scratch directories with good-old-c
#files.  This is used to port to another machine.

fillscratch:
	make -C src szal.result y.tab.cpp yystype.h
	cp src/_stdio.c scratch/src/
	cd scratch/src; $(CC) -I../../src         -I../../incl -Fc -..c ../../src/*.cpp;
	cd scratch/lib; $(CC) -I../../lib/complex -I../../incl -Fc -..c ../../lib/new/*.cpp
	cd scratch/lib; $(CC) -I../../lib/complex -I../../incl -Fc -..c ../../lib/static/*.cpp
	cp _munch/*.c scratch/mnch/

always:

clean:
	$(MAKE) -C src clean
	$(MAKE) -C lib/mk clean
	$(MAKE) -C demo/hello clean
	rm -f munch libC.a

