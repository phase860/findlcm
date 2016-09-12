#include "worker.hpp"
#include "test_helper.hpp"

#include <iostream>
#include <random>


static int testNumber = 0; 

void test1(std::vector<Number> x, std::vector<Number> trueAnswer) {
    MsgManager<std::vector<Number>> mm;
    Worker worker1(&mm);

    worker1.setFactorizationTask(x);

    worker1.setFactorizationTask(std::vector<Number>{}); // send empty vector
    auto answer = worker1.getData();

    std::vector<Number> vec;
    mapToVector(answer, vec);

    if (vec == trueAnswer) {
        std::cout << "[" << testNumber << "] " << "Test OK" << std::endl;
    } else {
        std::cout << "[" << testNumber << "] " << "Test FAIL" << std::endl;
        std::cout << "Numbers to find LCM: ";
        showVector(x);
        std::cout << "Answer: ";
        showMap(answer);
        std::cout << "True answer: ";
        showVector(trueAnswer);
        exit(0);
    }

    ++testNumber;
}


void test2(std::mt19937_64 &generator, int count) {
    MsgManager<std::vector<Number>> mm;
    Worker worker1(&mm);
    
    // because of checking strategy (find real Least Common Multiple
    // by multiply all factors) we will get overflow for big numbers
    std::uniform_int_distribution<int> dist(2, 100);

    std::vector<Number> x;
    for (int i=0; i < count; ++i) {
        x.push_back(dist(generator));
    }

    std::cout << "[" << testNumber << "] Process: ";
    showVector(x);

    worker1.setFactorizationTask(x);

    worker1.setFactorizationTask(std::vector<Number>{});
    auto answer = worker1.getData();

    // Checking
    Number lcm = 1;  
    for(auto it = answer.begin(); it != answer.end(); ++it) {
        for(uint64_t i=0; i < it->second; ++i) {
            lcm *= it->first; 
        }
    }

    bool testPass = true;
    for (auto it = x.begin(); it != x.end(); ++it) {
        if (lcm % (*it) != 0) { // check if testX is LCM
            testPass = false;
            break;
        }
    }

    if (testPass) {
        std::cout << "--- LCM: " << lcm << std::endl;
        std::cout << "--- " << "Test seems OK" << std::endl;
    } else {
        std::cout << "[" << testNumber << "] " << "Test FAIL" << std::endl;
        std::cout << "Input: ";
        showVector(x);

        std::cout << "Answer: ";
        showMap(answer);

        std::cout << "LCM: " << lcm << std::endl;

        exit(0);
    }

    ++testNumber;
}

void test3() {
    MsgManager<std::vector<Number>> mm;
    Worker worker1(&mm);
    worker1.setFactorizationTask(std::vector<Number>{});
    auto answer = worker1.getData();
    std::vector<Number> vec;
    mapToVector(answer, vec);

    if (vec.size() == 0) {
        std::cout << "[" << testNumber << "] " << "Test OK" << std::endl;
    } else {
        std::cout << "[" << testNumber << "] " << "Test FAIL" << std::endl;
        std::cout << "No values input test";
        exit(0);
    }

    ++testNumber;
}


int main() {
    // random generator init
    std::mt19937_64 generator((unsigned int) 
            std::chrono::system_clock::now().time_since_epoch().count());

    test1(std::vector<Number> {1}, std::vector<Number> {}); // special case
    test1(std::vector<Number> {2}, std::vector<Number> {2});
    test1(std::vector<Number> {3}, std::vector<Number> {3});
    test1(std::vector<Number> {4}, std::vector<Number> {2, 2});
    test1(std::vector<Number> {6}, std::vector<Number> {2, 3});
    test1(std::vector<Number> {100}, std::vector<Number> {2, 2, 5, 5});

    test1(std::vector<Number> {2, 3}, std::vector<Number> {2, 3});
    test1(std::vector<Number> {3, 2}, std::vector<Number> {2, 3});
    test1(std::vector<Number> {2, 2}, std::vector<Number> {2});
    test1(std::vector<Number> {7, 7, 7}, std::vector<Number> {7});
    test1(std::vector<Number> {100, 2, 5}, std::vector<Number> {2, 2, 5, 5});

    { // some random tests
        std::uniform_int_distribution<int> dist(1, 5);
        for(int i = 0; i < 10; ++i) {
            test2(generator, dist(generator));
        }
    }

    test3(); // no input test

    { // performance
        std::cout << "Performance test" << std::endl;

        MsgManager<std::vector<Number>> mm;
        Worker worker1(&mm);
        std::map<uint64_t, uint64_t> answer; 

        auto begin = std::chrono::system_clock::now();

        const int max = 10000000;
        for (int i=2; i<= max; ++i) {
            worker1.setFactorizationTask(
                    std::vector<Number>{static_cast<Number> (i)}); 
        }
        worker1.setFactorizationTask(std::vector<Number>{});
        answer = worker1.getData();

        auto end = std::chrono::system_clock::now();
        std::cout << "Find LCM first " << max << " numbers in " <<
            std::chrono::duration_cast<std::chrono::microseconds>
            (end - begin).count()/1000000 << " sec"  <<std::endl;
    }
                                  
    std::cout << std::endl << "All tests OK" << std::endl;

    return 0;
}
