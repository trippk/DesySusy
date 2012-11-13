all: mt2_bisect.o  mt2_bisect_sl.o 
	@echo "Compiled MT2_BISECT!"

%.o : %.cpp
	g++ -c -o $@ $<
%_sl.o : %.cpp
	g++ -fPIC -c -o $@ $<

clean:
	rm -f *.o *~
