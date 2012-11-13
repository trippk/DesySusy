ROOTINC  = `root-config --cflags`
CFLAGS   = $(ROOTINC) -I ../WIMPMASS
CC = g++

all:	topology22 topology33

topology22: topology22.cpp ../WIMPMASS/WIMPMASS.h
	$(CC) -I ../WIMPMASS  -c -o topology22.o topology22.cpp

topology33: topology33.cpp ../WIMPMASS/WIMPMASS.h	
	$(CC) $(CFLAGS) -c -o topology33.o topology33.cpp

clean:
	rm -f *.o *~ 

