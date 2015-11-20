CC 	=g++
BIN	=./bin/
SRC	=./src/

io	=io
iMCs01	=iMCs01
Arduino =Arduino
MAIN	= main
LRF	= UTM-30LX
GPS	= gps
CONT	= PS3

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

$(CONT:%=./bin/%.o):$(CONT:%=./src/%.cpp)
	$(CC) -c $< -o $@ $(LIB) -lm

$(MAIN):$(MAIN:%=./bin/%.o) $(io:%=./bin/%.o) $(CONT:%=./bin/%.o)
	$(CC) $(MAIN:%=./bin/%.o) $(io:%=./bin/%.o) $(CONT:%=./bin/%.o) -o $@ $(LIB)

clean:
	rm $(MAIN:%=./bin/%.o) $(io:%=./bin/%.o) $(CONT:%=./bin/%.o) $(MAIN)
