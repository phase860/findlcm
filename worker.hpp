#include "factorization.hpp"
#include "msgmanager.hpp"

#include <thread>
#include <vector>

/* Class worker create one thread with predefined factorization function.
 * When worker created it ready to process one input number with method
 * setFactorizationTask(). It has 'queue' with one number which can be set
 * while previouse number in process or complete to factorize. 
 * If we will set task while 'queue' not ready, our main process will block
 * and wait while current task will be complete. 
 * When we send empty vector to setFactorizationTask() it mean finish work and 
 * stop thread.
 */
class Worker {
public:
    Worker();
    Worker(MsgManager<std::vector<Number>> *msgmanager);
    ~Worker();
    void setFactorizationTask(std::vector<Number>);
    std::map<uint64_t, uint64_t> getData();

private:                                       
    int worker(); 

    MsgManager<std::vector<Number>> *_msgmanager;
    std::vector<Number> _task;
    std::map<uint64_t, uint64_t> _answer;

    std::thread _thread; 
};

