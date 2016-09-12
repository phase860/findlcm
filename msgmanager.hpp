#include <condition_variable>


template<class T>
class MsgManager {
public:
    MsgManager();
    void send(T msg);  // Send will be blocked if receiver not ready
    bool sendNoBlock(T msg);
    T receive(); // block if nothing to receive

private:
    T _msg;
    std::condition_variable _haveNewDataCondition;
    std::mutex _haveNewDataMutex;
    bool _haveNewData;

    std::condition_variable _haveReadyToReceiveCondition;
    std::mutex _haveReadyToReceiveMutex;
    bool _haveReadyToReceive; // Need for sendNoBlock
};


template<class T> 
MsgManager<T>::MsgManager() : _haveNewData(false), _haveReadyToReceive(true) {}


// Send will be blocked if receiver not ready
template<class T> 
void MsgManager<T>::send(T msg) {
    // lock if receiver not ready
    {                                                           
        std::unique_lock<std::mutex> lk(_haveReadyToReceiveMutex);
        _haveReadyToReceiveCondition.wait(lk, 
                [this]{ return _haveReadyToReceive; });
            _haveReadyToReceive = false;
    }

    // send message
    {
        std::lock_guard<std::mutex> g(_haveNewDataMutex);
        _haveNewData = true;
        _msg = msg; 
    }
    _haveNewDataCondition.notify_one();
}


// Receiver will be blocked if nothing to read
template<class T> 
T MsgManager<T>::receive() {
    T msg;
    {
        std::unique_lock<std::mutex> lk(_haveNewDataMutex);
        _haveNewDataCondition.wait(lk, [this]{ return _haveNewData; });
        msg = _msg; 
        _haveNewData = false; // ready to get new data
    }

    {
        std::lock_guard<std::mutex> g(_haveReadyToReceiveMutex);
        _haveReadyToReceive = true;
    }
    _haveReadyToReceiveCondition.notify_one();

    return msg;
}

