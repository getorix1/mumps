###################################################################
#
#	make 
#	make server
#
#	make install
#	
#
# May 12, 2024
#
###################################################################

SHELL       = /bin/sh
INSTALL     = install
PREFIX	= /usr
CC	= gcc
CXX	= g++

INCLUDEDIR  = $(PREFIX)/include
BINDIR      = $(PREFIX)/bin
LIBDIR      = $(PREFIX)/lib
SHARE       = $(PREFIX)/share
DOCDIR      = $(PREFIX)/share/doc/mumpsc
MANDIR      = $(PREFIX)/share/man
MAN1DIR     = $(MANDIR)/man1
SRCDIR      = mumpsc

#--------------------------------------------------------------------
# some recent versions of gcc/g++ have had problems with -o3 enabled
#--------------------------------------------------------------------

CFLAGS    = -w -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -O3 -fno-stack-protector -Iinclude/  \
	-finline-functions 

CPPFLAGS    = -w -fno-stack-protector `pkg-config --cflags --libs gtk+-3.0` -export-dynamic \
	`pkg-config --cflags --libs gdkmm-3.0`\
	-Iinclude/ -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE  -lgmp 

CPPOPTFLGS  = -w  -Ofast  -fno-stack-protector \
	`pkg-config --cflags --libs gtk+-3.0` -export-dynamic \
	`pkg-config --cflags --libs gdkmm-3.0`\
	-Iinclude/ -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE  -lgmp 

CPPSPCFLGS  = -w -falign-functions  -falign-jumps -falign-labels  -falign-loops -fauto-inc-dec -fbranch-count-reg -fcaller-saves -fcode-hoisting -fcombine-stack-adjustments -fcompare-elim -fcprop-registers -fcrossjumping -fcse-follow-jumps  -fcse-skip-blocks -fdce -fdefer-pop -fdelete-null-pointer-checks -fdevirtualize  -fdevirtualize-speculatively  -fdse -fforward-propagate -ffinite-loops -fgcse -fgcse-lm -fhoist-adjacent-loads -fif-conversion -fif-conversion2 -finline-functions -finline-functions-called-once -finline-small-functions -fipa-modref -fipa-profile -fipa-pure-const -fipa-reference -fipa-reference-addressable -fmerge-constants -fmove-loop-invariants -fomit-frame-pointer -freorder-blocks -fshrink-wrap -fshrink-wrap-separate -fsplit-wide-types -fssa-backprop -fssa-phiopt -ftree-bit-ccp -ftree-ccp -ftree-ch -ftree-coalesce-vars -ftree-copy-prop -ftree-dce -ftree-dominator-opts -ftree-dse -ftree-forwprop -ftree-fre -ftree-phiprop -ftree-pta -ftree-scev-cprop -ftree-sink -ftree-slsr -ftree-sra -ftree-ter -funit-at-a-time   \
	-fno-stack-protector `pkg-config --cflags --libs gtk+-3.0` -export-dynamic \
	`pkg-config --cflags --libs gdkmm-3.0`\
	-Iinclude/ -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE  -lgmp 

HEADERS   = include/mumpsc/*.h

BINARIES  = mumps2c 

LIBRARIES = libmumps.a libmpsglobal_native.a libmpscpp.a 

all: $(BINARIES) $(LIBRARIES) 

############################################################################
#                                  Binaries                                #
############################################################################

MUMPS2C_OBJS    = mumps.o parse.o sys1.o at-test.o copyright-notice.o \
	compiler-open.o set.o for.o abrtx.o compiler-lock.o if.o kill.o new.o read.o \
	write.o zcommands.o doty.o fcninit.o epi.o globalarray.o localarray.o 

localarray.o : localarray.c $(HEADERS)
	$(CC) -c localarray.c $(CFLAGS)

globalarray.o : globalarray.c $(HEADERS)
	$(CC) -c globalarray.c $(CFLAGS)

epi.o : epi.c $(HEADERS)
	$(CC) -c epi.c $(CFLAGS)

fcninit.o : fcninit.c $(HEADERS)
	$(CC) -c fcninit.c $(CFLAGS)

doty.o : doty.c $(HEADERS)
	$(CC) -c doty.c $(CFLAGS)

zcommands.o : zcommands.c $(HEADERS)
	$(CC) -c zcommands.c $(CFLAGS)

write.o : write.c $(HEADERS)
	$(CC) -c write.c $(CFLAGS)

read.o : read.c $(HEADERS)
	$(CC) -c read.c $(CFLAGS)

new.o : new.c $(HEADERS)
	$(CC) -c new.c $(CFLAGS)

kill.o : kill.c $(HEADERS)
	$(CC) -c kill.c $(CFLAGS)

if.o : if.c $(HEADERS)
	$(CC) -c if.c $(CFLAGS)

compiler-lock.o : compiler-lock.c $(HEADERS)
	$(CC) -c compiler-lock.c $(CFLAGS)

abrtx.o : abrtx.c $(HEADERS)
	$(CC) -c abrtx.c $(CFLAGS)

for.o : for.c $(HEADERS)
	$(CC) -c for.c $(CFLAGS)

set.o : set.c $(HEADERS)
	$(CC) -c set.c $(CFLAGS)

copyright-notice.o : copyright-notice.c $(HEADERS)
	$(CC) -c copyright-notice.c $(CFLAGS)

at-test.o : at-test.c $(HEADERS)
	$(CC) -c at-test.c $(CFLAGS)

mumps2c	: $(MUMPS2C_OBJS) $(HEADERS)
	$(CC) -o mumps2c $(MUMPS2C_OBJS) $(CFLAGS)

############################################################################
#                                 Libraries                                #
############################################################################

libmpscpp.o : libmpscpp.cpp
	g++ -c libmpscpp.cpp $(CPPOPTFLGS)

libmpscpp.a : libmpscpp.o
	ar rcs libmpscpp.a libmpscpp.o 

#copyright-notice.o : copyright-notice.cpp $(HEADERS)
#	g++ -c copyright-notice.cpp $(CFLAGS)

arith.o : arith.cpp $(HEADERS)
	g++ -c arith.cpp $(CPPOPTFLGS)

bifs.o : bifs.cpp $(HEADERS)
	g++ -c bifs.cpp $(CPPOPTFLGS)

bstring.o : bstring.cpp $(HEADERS)
	g++ -c bstring.cpp $(CPPOPTFLGS)

interp.o : interp.cpp $(HEADERS)
	g++ -c interp.cpp $(CPPOPTFLGS)

pm.o : pm.cpp $(HEADERS)
	g++ -c pm.cpp $(CPPOPTFLGS)

stem.o : stem.cpp $(HEADERS)
	g++ -c stem.cpp $(CPPOPTFLGS)

strmanip.o : strmanip.cpp $(HEADERS)
	g++ -c strmanip.cpp $(CPPOPTFLGS)

sym.o : sym.cpp $(HEADERS)
	g++ -c sym.cpp $(CPPOPTFLGS)

sysfunc.o : sysfunc.cpp $(HEADERS)
	g++ -c sysfunc.cpp $(CPPOPTFLGS)

lock.o : lock.cpp $(HEADERS)
	g++ -c lock.cpp $(CPPOPTFLGS)

bmgsubs.o : bmgsubs.c $(HEADERS)
	gcc -c bmgsubs.c $(CPPOPTFLGS)

libmumps.a : arith.o bifs.o bstring.o interp.o pm.o stem.o \
		strmanip.o sym.o sysfunc.o lock.o bmgsubs.o
	ar rcs libmumps.a arith.o bifs.o bstring.o interp.o pm.o stem.o  \
		strmanip.o sym.o sysfunc.o lock.o bmgsubs.o


globalb.o : globalb.cpp $(HEADERS)
	g++ -c globalb.cpp $(CPPOPTFLGS)

btree.o : btree.cpp $(HEADERS)
	g++ -c btree.cpp $(CPPSPCFLGS)

libmpsglobal_native.a : globalb.o btree.o
	ar rcs libmpsglobal_native.a globalb.o btree.o


############################################################################

install: install-common

install-common:
	$(INSTALL) -d        $(PREFIX)
	$(INSTALL) -d        $(BINDIR)
	$(INSTALL) -d        $(INCLUDEDIR)
	$(INSTALL) -d        $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -d        $(SHARE)
	$(INSTALL) -d        $(SHARE)/doc/
	$(INSTALL) -d        $(SHARE)/man/
	$(INSTALL) -d        $(MANDIR)
	$(INSTALL) -d        $(MAN1DIR)
	$(INSTALL) -d        $(LIBDIR)
	$(INSTALL) -d        $(DOCDIR)
	$(INSTALL) -m 755 -s $(BINARIES)               $(BINDIR)
	$(INSTALL) -m 755    mumpsc                    $(BINDIR)
	$(INSTALL) -m 755    mumpslib                  $(BINDIR)
	$(INSTALL) -m 755    CreateSqliteDB.script     $(BINDIR)
	$(INSTALL) -m 644    $(LIBRARIES)              $(LIBDIR)



	$(INSTALL) -m 644    include/mumpsc/arith.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/errors.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/bifs.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/bstring.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/btree.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/builtin.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/cgi.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/config.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/cppsysfunc.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/externs.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/fcns.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/defines.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/global.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/globalClass.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/mstringClass.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/globalOrder.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/globalvars.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/interp.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/keyfix.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/compiler.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/libmpscpp.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/memsize.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/numSetupDefines.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/numSetup.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/pm.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/stateVector.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/stem.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/strmanip.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/strmax.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/inline.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/sym.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/sysfunc.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/sysparms.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/symbasic.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    include/mumpsc/builtinTable.h $(INCLUDEDIR)/mumpsc/
	$(INSTALL) -m 644    Documentation/MDH-UsersGuide.pdf $(DOCDIR)
	$(INSTALL) -m 644    Documentation/MumpsTutorial.pdf $(DOCDIR)
	$(INSTALL) -m 644    Documentation/MumpsUsersGuide.pdf $(DOCDIR)

	$(INSTALL) -d        $(DOCDIR)/examples/
	$(INSTALL) -d        $(MAN1DIR)

	chmod u+x mumpsc
	./mumpsc mumps.cpp
	$(INSTALL) -m 755  mumps                       $(BINDIR)

uninstall:
	cd $(BINDIR) && $(RM) $(BINARIES) mumpsc mumps mumps2c mumpsRO mumpslib
	$(RM) -r $(DOCDIR)
	$(RM) -r $(INCLUDEDIR)/mumpsc/

clean:
	$(RM) *.[oas] $(BINARIES)
	rm -f key.dat
	rm -f data.dat
	rm -f mumps mumpsc mumpsRO mumps2c
	rm -f Checkout/examples/btree/btree1
	rm -f Checkout/examples/btree/btree2
	rm -f Checkout/examples/btree/btree3
	rm -f Checkout/examples/btree/btree4
	rm -f Checkout/examples/btree/btree5
	rm -f Checkout/examples/btree/btree6
	rm -f Checkout/examples/btree/btree7
	rm -f Checkout/examples/btree/btree8
	rm -f Checkout/examples/btree/data.dat
	rm -f Checkout/examples/btree/key.dat
	rm -f Checkout/validate-compiler/key.dat
	rm -f Checkout/validate-compiler/data.dat
	rm -f Checkout/validate-compiler/*.cpp
	rm -f Checkout/validate-compiler/*.orig
	rm -f Checkout/validate-compiler/ccc.*
	rm -f Checkout/validate-compiler/ddd.*
	rm -f Checkout/validate-compiler/eee.*
	rm -f Checkout/validate-compiler/ggg.*
	rm -f Checkout/validate-compiler/hhh.*
	rm -f Checkout/validate-compiler/backref
	rm -f Checkout/validate-compiler/checkout
	rm -f Checkout/validate-compiler/do1
	rm -f Checkout/validate-compiler/extractTests
	rm -f Checkout/validate-compiler/fcns
	rm -f Checkout/validate-compiler/for1
	rm -f Checkout/validate-compiler/for2
	rm -f Checkout/validate-compiler/for3
	rm -f Checkout/validate-compiler/globals1
	rm -f Checkout/validate-compiler/go1
	rm -f Checkout/validate-compiler/kill
	rm -f Checkout/validate-compiler/lclOrder
	rm -f Checkout/validate-compiler/new01
	rm -f Checkout/validate-compiler/new02
	rm -f Checkout/validate-compiler/new03
	rm -f Checkout/validate-compiler/ops
	rm -f Checkout/validate-compiler/pieceTests
	rm -f Checkout/validate-compiler/qtst
	rm -f Checkout/validate-compiler/remote
	rm -f Checkout/validate-compiler/s16a
	rm -f Checkout/validate-compiler/s6a
	rm -f Checkout/validate-compiler/s6b
	rm -f Checkout/validate-compiler/s6c
	rm -f Checkout/validate-compiler/s6
	rm -f Checkout/validate-compiler/s6d
	rm -f Checkout/validate-compiler/zfunctions

	rm -f *.orig
