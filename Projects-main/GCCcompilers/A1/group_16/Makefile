CXX   = g++-8
EXE = iplC
CXXDEBUG = -g
CXXSTD = -std=c++11


.PHONY: all
all: parser lexer 	
	$(CXX) $(CXXDEBUG) -o $(EXE) driver.cpp parser.o scanner.o 

parser: parser.yy scanner.hh
	bison -dv $<
	$(CXX) $(CXXDEBUG) -c parser.tab.cc -o parser.o 

lexer: scanner.l scanner.hh parser.tab.hh parser.tab.cc	
	flex++ --outfile=scanner.yy.cc  $<
	$(CXX)  $(CXXDEBUG) -c scanner.yy.cc -o scanner.o

clean:
	mv scanner.hh scan || true
	rm *.cc *.o *.output *.hh $(EXE) || true
	mv scan scanner.hh || true
