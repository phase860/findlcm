CXX=g++
CXXFLAGS=-g -std=c++0x -Wall -pedantic 
# -O3
BIN=main

SRC=$(wildcard *.cpp)
OBJ=$(SRC:%.cpp=%.o)


$(BIN): $(BIN).o threadpool.o factorization.o worker.o msgmanager.hpp
	$(CXX) $(BIN).o threadpool.o factorization.o worker.o -o $(BIN) -pthread

main_test: main_test.o worker.o test_helper.o factorization.o
	$(CXX) worker.o factorization.o test_helper.o main_test.o -o main_test -pthread

generator: generator.o
	$(CXX) generator.o -o generator

threadpool_test: threadpool.o threadpool_test.o worker.o factorization.o msgmanager.hpp test_helper.o
	$(CXX) threadpool.o threadpool_test.o worker.o factorization.o test_helper.o -o threadpool_test -pthread

worker_test: worker.o worker_test.o factorization.o test_helper.o msgmanager.hpp
	$(CXX) worker.o worker_test.o factorization.o test_helper.o -o worker_test -pthread

msgmanager_test: msgmanager_test.o test_helper.o msgmanager.hpp
	$(CXX) msgmanager_test.o test_helper.o -o msgmanager_test


factorization_test: factorization_test.o factorization.o test_helper.o 
	$(CXX) factorization.o factorization_test.o test_helper.o -o factorization_test

%.o: %.cpp %.hpp factorization.hpp
	$(CXX) $(CXXFLAGS) -c $< 

.PHONY : clean
clean:
	-rm -- *.o $(BIN) tags
	-rm factorization_test msgmanager_test worker_test threadpool_test main_test
	-rm generator
	-rm -r *.dSYM
