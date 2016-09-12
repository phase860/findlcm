#include "worker.hpp"


int Worker::worker() {
    while (true) {
        std::vector<Number> task = _msgmanager->receive(); // blocking here

        // empty input - command to exit
        if (task.empty()) {  
            return 0;
        }
        
        for (auto it = task.begin(); it != task.end(); ++it) {
            auto answer = std::map<uint64_t, uint64_t>{};
            factorization(*it, answer);

            // update Worker map
            for (auto it = answer.begin(); it != answer.end(); ++it) {
                _answer[it->first] = std::max(_answer[it->first], it->second);
            }
        }
    }

    return 0;
}


Worker::Worker(MsgManager<std::vector<Number>> *msgmanager):
    _msgmanager(msgmanager), _thread(&Worker::worker, this) {} 


Worker::~Worker() {}


void Worker::setFactorizationTask(std::vector<Number> task) {
    _msgmanager->send(task);
}


// getData wait until unprocessed data will be processed, finish thread 
// and return data
std::map<uint64_t, uint64_t> Worker::getData() {
    _thread.join();
    return _answer;
}


























