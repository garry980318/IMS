CC = g++

all:ims-projekt

ims-projekt: plasty.cpp
	$(CC) $^ -o $@ -lsimlib

run:
	make  && ./ims-projekt $(ARGS)

clean:
	rm -f *.o ims-projekt
