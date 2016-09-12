#include "threadpool.hpp"

#include <exception>


constexpr size_t bufferSize = 1000;  

const char* NotValidInput::what() const throw() {
    return "Not valid input value";
}


NotValidInput notValidInput; // exception


ThreadPool::ThreadPool(size_t threadsamount) {
    for(uint64_t i=0; i < threadsamount; ++i) {
        auto newthread = std::make_shared<Worker>(&msgmanager);
        _threads.push_back(std::move(newthread));
    }

}


ThreadPool::~ThreadPool() { } 


void ThreadPool::processData(Number x) {
    if (x < 1) {
        throw notValidInput;
    }

    data.push_back(x);

    if (bufferSize < data.size()) {
        msgmanager.send(data);
        data = std::vector<Number>{};
    }
}


void ThreadPool::getAnswer(std::map<uint64_t, uint64_t> &answer) {
    if (false == data.empty()) {
        msgmanager.send(data); // if we have unsended data
    }

    for (auto it = _threads.begin(); it != _threads.end(); ++it) {
        msgmanager.send(std::vector<Number>{}); // please finish all threads
    }

    for (auto it = _threads.begin(); it != _threads.end(); ++it) {
        auto dataFromWorker = (*it)->getData();

        for(auto it = dataFromWorker.begin(); it != dataFromWorker.end(); ++it) {
            answer[it->first] = std::max(answer[it->first], it->second); 
        }
     }
}

