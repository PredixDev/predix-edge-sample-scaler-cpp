CFLAGS=-Wall -std=c++11
LDFLAGS= -lz -lmosquittopp
IFLAGS=-I/usr/local/include/cget/include -I/usr/local/include -I/usr/include

.PHONY: all clean

all: scaler_app

scaler_app: main.o mqtt_client.o
	ls /; \
	cd ./src; \
	${CXX} $^ -o $@ ${LDFLAGS} ${CFLAGS}

mqtt_client.o:
	cd ./src/; \
	${CXX} ${IFLAGS} -c mqtt_client.cpp -o $@ ${CFLAGS}

main.o:
	cd ./src; \
	${CXX} ${IFLAGS} -c main.cpp -o $@ ${CFLAGS}

clean:
	cd ./src; \
	rm -f *.o scaler_app
