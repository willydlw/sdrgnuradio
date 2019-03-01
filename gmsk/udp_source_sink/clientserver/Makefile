txobjects = txclient.o txrxhelper.o 

rxobjects = rxserver.o txrxhelper.o

all: etxclient erxserver

etxclient: $(txobjects) 
	gcc -o etxclient txclient.o txrxhelper.o

erxserver: $(rxobjects)
	gcc -o erxserver rxserver.o  txrxhelper.o


txclient.o: txclient.c txrxhelper.h
	gcc -Wall -Wextra -c -std=gnu11 txclient.c

txrxhelper.o: txrxhelper.c txrxhelper.h
	gcc -Wall -Wextra -c -std=gnu11 txrxhelper.c


# using a Phony target prevents make from getting confused
# by an actual file named clean
.PHONY : clean
clean:
	rm -f etxclient erxserver $(txobjects) $(rxobjects)


