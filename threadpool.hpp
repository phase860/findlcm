#include "worker.hpp"


// Exception for problems with input
class NotValidInput : public std::exception {
public:
    virtual const char* what() const throw(); 
};


class ThreadPool {
public:
    ThreadPool(size_t threadamount);
    ~ThreadPool(); 
    void processData(Number); 
    void getAnswer(std::map<uint64_t, uint64_t> &);

private:
    std::shared_ptr<Worker> pointer;
    std::vector<std::shared_ptr<Worker>> _threads;
    MsgManager<std::vector<Number>> msgmanager;
    std::vector<Number> data;
};                        
