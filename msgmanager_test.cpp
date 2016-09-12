#include "msgmanager.hpp"
#include "test_helper.hpp"

#include <iostream>
#include <thread>


MsgManager<int> mm;

void producer() {
    std::cout << "producer in work" << std::endl;
    mm.send(10);
    std::cout << "producer send msg" << std::endl;
}

void consumer() {

    std::cout << "consumer in work" << std::endl;
    std::cout << mm.receive() << std::endl;
}



int main() {
    std::cout << "hello" << std::endl;

    producer();

//    auto t1 = std::thread(producer);
    auto t2 = std::thread(consumer);


//    t1.join();
    t2.join();



    return 0;
}
