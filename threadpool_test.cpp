#include "threadpool.hpp"
#include "test_helper.hpp"

#include <iostream>
#include <random>

static int testNumber = 0; 

void test1(std::vector<Number> x, std::vector<Number> trueAnswer) {
    ThreadPool tp(4);

    for(auto it = x.begin(); it != x.end(); ++it) {
        tp.processData(*it); 
    }

    std::map<uint64_t, uint64_t> answer;
    tp.getAnswer(answer);

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
    ThreadPool tp(4);
    
    // because of checking strategy (find real Least Common Multiple
    // by multiply all factors) we will get overflow for big numbers
    std::uniform_int_distribution<int> dist(2, 100);

    std::vector<Number> x;
    for (int i=0; i < count; ++i) {
        x.push_back(dist(generator));
    }

    std::cout << "[" << testNumber << "] Process: ";
    showVector(x);

    for(auto it = x.begin(); it != x.end(); ++it) {
        tp.processData(*it); 
    }

    std::map<uint64_t, uint64_t> answer;
    tp.getAnswer(answer);

    // Checking
    Number lcm = 1;  
    for(auto it = answer.begin(); it != answer.end(); ++it) {
        for(uint64_t i=0; i < it->second; ++i)  lcm *= it->first; 
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
    ThreadPool tp(4);

    std::map<uint64_t, uint64_t> answer;
    tp.getAnswer(answer);

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

    test3(); // empty input test


    std::map<uint64_t, uint64_t> answerThreadPool; 
    const int max = 10000000;
    { // performance
        std::cout << "Performance test" << std::endl;
        ThreadPool tp(4);

        auto begin = std::chrono::system_clock::now();

        for (int i=2; i<= max; ++i) {
            tp.processData(i); 
        }

        tp.getAnswer(answerThreadPool);

        auto end = std::chrono::system_clock::now();
        std::cout << "ThreadPool Find LCM first " << max << " numbers in " <<
            std::chrono::duration_cast<std::chrono::microseconds>
            (end - begin).count()/1000000 << " sec"  <<std::endl;
    }

    {
        std::cout << "Check if result is correct" << std::endl;

        MsgManager<std::vector<Number>> mm;
        Worker worker1(&mm);
        std::map<uint64_t, uint64_t> answerWorker; 

        auto begin = std::chrono::system_clock::now();

        for (int i=2; i<= max; ++i) {
            worker1.setFactorizationTask(
                    std::vector<Number>{static_cast<Number> (i)}); 
        }

        worker1.setFactorizationTask(std::vector<Number>{});
        answerWorker = worker1.getData();

        auto end = std::chrono::system_clock::now();
        std::cout << "Worker Find LCM first " << max << " numbers in " <<
            std::chrono::duration_cast<std::chrono::microseconds>
            (end - begin).count()/1000000 << " sec"  <<std::endl;

        if (answerWorker == answerThreadPool) {
            std::cout << "Correctness Test OK" << std::endl;
        } else {
            std::cout << "Correctness Test FAIL" << std::endl;
            exit(0);
        }
    }
                                  
    std::cout << std::endl << "All tests OK" << std::endl;

}
