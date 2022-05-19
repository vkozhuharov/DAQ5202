########################################################################
#
##              --- CAEN SpA - Computing Division ---
#
##   CAENDigitizer Software Project
#
##   Created  :  October    2009      (Rel. 1.0)
#
##   Auth: A. Lucchesi
#
#########################################################################
ARCH	=	`uname -m`

OUTDIR  =    	./bin/
OUTNAME =    	DAQ
OUT     =    	$(OUTDIR)/$(OUTNAME)

CC	=	g++

COPTS	=	-fPIC -DLINUX -O2

#FLAGS	=	-soname -s
#FLAGS	=       -Wall,-soname -s
#FLAGS	=	-Wall,-soname -nostartfiles -s
#FLAGS	=	-Wall,-soname

DEPLIBS	=	

LIBS	=	-L.

INCLUDEDIR =	-I./inc

OBJS	=	DAQ.o src/DAQVConfig.o src/DT5202Config.o  src/DT5202.o  src/utlConfigParser.o  src/utlMessageBus.o

INCLUDES =	./inc/*

#########################################################################

all	:	$(OUT)

clean	:
		/bin/rm -f $(OBJS) $(OUT)

$(OUT)	:	$(OBJS)
		/bin/rm -f $(OUT)
		if [ ! -d $(OUTDIR) ]; then mkdir -p $(OUTDIR); fi
		$(CC) $(FLAGS) -o $(OUT) $(OBJS) $(DEPLIBS)

$(OBJS)	:	$(INCLUDES) Makefile


%.o	:	%.cc
		$(CC) $(COPTS) $(INCLUDEDIR) -c -o $@ $<

