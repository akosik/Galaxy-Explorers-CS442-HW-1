CC=gcc

CFLAGS=-Wall

SOURCES= eaf.c ead.c ecd.c ecf.c evf.c evd.c
EXECUTABLES= $(basename $(SOURCES))

all: assembly vector plainc

assembly: afloat adouble

afloat:
	$(CC) $(CFLAGS) eaf.c -o eaf
adouble:
	$(CC) $(CFLAGS) ead.c -o ead

plainc: cfloat cdouble

cfloat:
	$(CC) $(CFLAGS) ecf.c -o ecf
cdouble:
	$(CC) $(CFLAGS) ecd.c -o ecd

vector: vfloat vdouble

vfloat:
	$(CC) $(CFLAGS) evf.c -o evf
vdouble:
	$(CC) $(CFLAGS) evd.c -o evd

clean:
	rm $(EXECUTABLES)
