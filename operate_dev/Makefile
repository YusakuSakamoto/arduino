CC 	=g++
BIN	=./bin/
SRC	=./src/

io	=io
iMCs01	=iMCs01
Arduino =Arduino
MAIN	= main
LRF		= UTM-30LX
GPS		= gps

LRF_OPTION	=`urg_c-config --cflags` `urg_c-config --libs`
LIB	=-lboost_system -lboost_filesystem -lpthread -std=c++11

.PHONY:clean
all:$(MAIN)

$(io:%=./bin/%.o):$(io:%=./src/%.cpp)
	$(CC) -c $< -o $@ $(LIB)

$(MAIN:%=./bin/%.o):$(MAIN:%=./src/%.cpp)
	$(CC) -c $< $(LRF_OPTION) -o $@ $(LIB)

$(GPS:%=./bin/%.o):$(GPS:%=./src/%.cpp)
	$(CC) -c $< -o $@ $(LIB)

$(iMCs01:%=./bin/%.o):$(iMCs01:%=./src/%.cpp)
	$(CC) -c $< -o $@ $(LIB)

$(Arduino:%=./bin/%.o):$(Arduino:%=./src/%.cpp)
	$(CC) -c $< -o $@ $(LIB)

$(LRF:%=./bin/%.o):$(LRF:%=./src/%.cpp)
	$(CC) -c $< $(LRF_OPTION) -o $@ $(LIB) -lm

$(MAIN):$(MAIN:%=./bin/%.o) $(Arduino:%=./bin/%.o) $(io:%=./bin/%.o) $(iMCs01:%=./bin/%.o) $(GPS:%=./bin/%.o) $(LRF:%=./bin/%.o)
	$(CC) $(MAIN:%=./bin/%.o) $(Arduino:%=./bin/%.o) $(io:%=./bin/%.o) $(iMCs01:%=./bin/%.o) $(GPS:%=./bin/%.o) $(LRF:%=./bin/%.o) $(LRF_OPTION) -o $@ $(LIB)

clean:
	rm $(MAIN:%=./bin/%.o) $(Arduino:%=./bin/%.o) $(io:%=./bin/%.o) $(iMCs01:%=./bin/%.o) $(LRF:%=./bin/%.o) $(GPS:%=./bin/%.o) $(MAIN)
