CXX = g++
CXXFLAGS = -std=c++11

main: main.o Parser.o CodeWriter.o
	$(CXX) $(CXXFLAGS) -o main main.o Parser.o CodeWriter.o

main.o: Parser.h CodeWriter.h
	$(CXX) $(CXXFLAGS) -c main.cpp

CodeWriter.o: CodeWriter.h
	$(CXX) $(CXXFLAGS) -c CodeWriter.cpp 

Parser.o: Parser.h
	$(CXX) $(CXXFLAGS) -c Parser.cpp

clean: 
	rm -f *.o
	rm -f main