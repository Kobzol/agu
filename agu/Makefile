
OPENCV=`pkg-config --cflags --libs opencv`

all: cviko1

cviko1: 
	g++ -std=c++11 $@.cpp lib/polygon.cpp lib/algo.cpp $(OPENCV) -o $@

clean:
	-rm cviko1
