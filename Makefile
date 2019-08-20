#
#    Author                  : Paul Onions
#    Creation date           : 9 May 2014
#
#    Copyright 2015 - 2019 Silicon Infusion Limited
#
#    Silicon Infusion Limited
#    CP House
#    Otterspool Way
#    Watford WD25 8HP
#    Hertfordshire, UK
#    Tel: +44 (0)1923 650404
#    Fax: +44 (0)1923 650374
#    Web: www.siliconinfusion.com
#
#    Licence: MIT, see LICENCE file for details.
#
PREFIX ?= /usr/local

EXTRA_CFLAGS += -std=c99
EXTRA_CFLAGS += -I.
EXTRA_CFLAGS += -Iad

AD_INCS = ad/common.h ad/util.h ad/ad9361.h ad/ad9361_api.h
AD_SRCS = ad/util.c ad/ad9361.c ad/ad9361_api.c
SI_INCS = platform.h libzaltys-ad9361.h
SI_SRCS = platform.c default_init.c

SRCS   = $(AD_SRCS) $(SI_SRCS)
OBJS   = $(SRCS:c=o)
SHOBJS = $(SRCS:c=so)
LIB    = libzaltys-ad9361.a
SHLIB  = libzaltys-ad9361.so
LIBS   = $(LIB) $(SHLIB)

all : $(AD_INCS) $(SI_INCS) $(LIBS)

$(LIB) : $(OBJS)
	ar rv $(LIB) $(OBJS)

$(SHLIB) : $(SHOBJS)
	$(CC) -shared -fPIC -o $(SHLIB) $(SHOBJS) -Wl,--version-script=libzaltys-ad9361.version

%.o : %.c
	$(CC) -c $(CFLAGS) $(EXTRA_CFLAGS) $< -o $@

%.so : %.c
	$(CC) -c $(CFLAGS) $(EXTRA_CFLAGS) -fPIC $< -o $@

install : all
	mkdir -p $(PREFIX)/include/zaltys
	mkdir -p $(PREFIX)/include/zaltys/ad9361
	mkdir -p $(PREFIX)/lib
	cp -a $(AD_INCS) $(PREFIX)/include/zaltys/ad9361
	cp -a libzaltys-ad9361.h $(PREFIX)/include/zaltys
	cp -a $(LIBS) $(PREFIX)/lib

clean :
	@rm -f *.o *.so *.a
	@rm -f ad/*.o ad/*.so
