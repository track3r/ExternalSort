CXX=g++
FLAGS=-std=c++11 -stdlib=libc++

ExternalSort: 
	$(CXX) $(FLAGS) main.cpp MultiFileHeap.cpp Io.cpp ParalellSplitSort.cpp Test.cpp -o ExternalSort
 
clean:
	rm -fr *.o ExternalSort


 
.PHONY: all clean test