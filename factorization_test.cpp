#include "factorization.hpp"
#include "test_helper.hpp"

#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <chrono>

static int testNumber = 0;


void test1(Number x, std::vector<Number> trueAnswer) {
    std::map<uint64_t, uint64_t> answer; 
    std::vector<Number> vec;

    factorization(x, answer);
    mapToVector(answer, vec);

    if (vec == trueAnswer) {
        std::cout << "[" << testNumber << "] " << "Test OK" << std::endl;
    } else {
        std::cout << "[" << testNumber << "] " << "Test FAIL" << std::endl;
        std::cout << "factorization( " << x << " ): ";
        showVector(vec);

        std::cout << "True answer: ";
        showVector(trueAnswer);
        exit(0);
    }

    ++testNumber;
}


void test2(std::mt19937_64 &generator) {
    Number x = generator();
    std::map<uint64_t, uint64_t> answer; 

    std::cout << "[" << testNumber << "] " <<
        "factorization( " << x << " ) " << std::endl;

    factorization(x, answer);
    Number testX = 1;  
    for(auto it = answer.begin(); it != answer.end(); ++it) {
        for(uint64_t i=0; i < it->second; ++i)  
            testX *= it->first;
    }

    if (testX == x) {
        //std::cout << "[" << testNumber << "] " << "Test seems OK" << std::endl;
        std::cout << "--- " << "Test seems OK" << std::endl;
    } else {
        std::cout << "[" << testNumber << "] " << "Test FAIL" << std::endl;
        std::cout << "factorization( " << x << " ): ";

        std::vector<Number> vec;
        mapToVector(answer, vec);
        showVector(vec);
        exit(0);
    }

    ++testNumber;
}


int main() {                        
    // random generator init
    std::mt19937_64 generator((unsigned int) 
            std::chrono::system_clock::now().time_since_epoch().count());

    test1(1, std::vector<Number> {}); // special case
    test1(2, std::vector<Number> {2});
    test1(3, std::vector<Number> {3});
    test1(4, std::vector<Number> {2, 2});
    test1(6, std::vector<Number> {2, 3});
    test1(100, std::vector<Number> {2, 2, 5, 5});

    for(int i = 0; i<3; ++i) {
        test2(generator);
    }
                                     

    std::cout << "Performance test" << std::endl;
    std::map<uint64_t, uint64_t> bigTable; 
    auto begin = std::chrono::system_clock::now();

    const int max = 10000000;
    for (int i=2; i<= max; ++i) {
        std::map<uint64_t, uint64_t> localAnswer; 
        factorization(i, localAnswer);
        for(auto it = localAnswer.begin(); it != localAnswer.end(); ++it) {
             bigTable[it->first] = std::max(bigTable[it->first], it->second); 
        }
    }
    auto end = std::chrono::system_clock::now();
    std::cout << "Factorize first " << max << " numbers in " <<
        std::chrono::duration_cast<std::chrono::microseconds>
        (end - begin).count()/1000000 << " sec"  <<std::endl;

     std::cout << std::endl << "All tests OK" << std::endl;

    return 0;
}


