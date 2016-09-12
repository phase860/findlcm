#include "worker.hpp"
#include "test_helper.hpp"

#include <iostream>


int main() {
    int inputNumber;
    MsgManager<std::vector<Number>> mm;
    Worker worker1(&mm);

    while (std::cin >> inputNumber) {
        worker1.setFactorizationTask(
                std::vector<Number>{static_cast<Number> (inputNumber)}); 
    }

    worker1.setFactorizationTask(std::vector<Number>{});
    auto answer = worker1.getData();

    showMap(answer);

    return 0;
}
