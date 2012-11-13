all:
	cd src; make

clean:
	rm -f *~
	cd src;make clean
	cd WIMPMASS;rm -f *~
tar:
	make clean
	cd ..;tar -cvzf WIMPMASS-1.00.tar.gz WIMPMASS
