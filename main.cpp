#include "threadpool.hpp"

#include <iostream>
#include <array>


constexpr auto threadsNumber = 4; // how many workers? 
constexpr uint64_t checkedMaxNumber = 10001; // for CheckForRepeated 

NotValidInput mainNotValidInput;


// Specific to exercise conditions.
// Max input number is 10000, so we can not to
// factorize number if it is already processed.
// This class return true if number appear first time
// and return false if number repeated. 
// Work for numbers from [0, maxNumber). For greater numbers
// always return true.
class CheckForRepeated {
public:
    CheckForRepeated() {
        firstTime.fill(true);
    }

    bool isNumberFirstTime(Number value) {
        if (checkedMaxNumber <= value) {
            return true;
        }

        if (firstTime[value]) {
            firstTime[value] = false;
            return true;
        } else {
            return false;
        }
    }

private:
    std::array<bool, checkedMaxNumber> firstTime;
};


// getchar_unlocked for fast reading input
// not thread safe
// https://habrahabr.ru/post/246257/
bool read_int_unlocked(int & out)
{
    int c = getchar_unlocked();
    int x = 0;
    int neg = 0;

    for (; !('0'<=c && c<='9') && c != '-'; c = getchar_unlocked()) {
        if (c == EOF)
            return false;
        else 
            throw mainNotValidInput;
    }

    if (c == '-') {
        neg = 1;
        c = getchar_unlocked();
    }

    if (c == EOF)
        return false;

    int prev = 0;
    for (; '0' <= c && c <= '9' ; c = getchar_unlocked()) {
        x = x*10 + c - '0';
        if (prev > x) { // overflow
            std::cout << "Overflow" << std::endl;
            throw mainNotValidInput;
        }
    }

    out = neg ? -x : x;
    return true;
}


int main() {
    int inputNumber;
    ThreadPool threadPool(threadsNumber);
    CheckForRepeated checker;

    try {
        while(read_int_unlocked(inputNumber)) {
            if (inputNumber < 1) {
                throw mainNotValidInput;
            }

            if (checker.isNumberFirstTime(inputNumber)) {
                threadPool.processData(inputNumber);
            }
        }
  
    } catch (const NotValidInput &exception) {
        std::cerr << "Exception: " << exception.what() << std::endl;
                               
        std::map <uint64_t, uint64_t> answer;
        threadPool.getAnswer(answer); // correct finish threads
        return 1;
    }

    std::map <uint64_t, uint64_t> answer;
    threadPool.getAnswer(answer); 

    for(auto it = answer.begin(); it != answer.end(); ++it) {
        for(uint64_t i=0; i < it->second; ++i) {
             std::cout << it->first << " ";
        }
    }
    std::cout << std::endl;

    return 0;
}
