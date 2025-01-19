#ifndef msr_semaphore_Hpp
#define msr_semaphore_Hpp

#include <mutex>

class Semaphore
{
public:
    explicit Semaphore(int count = 0) : _count(count)
    {
    }

    void Signal()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        ++_count;
        _cv.notify_one();
    }

    void Wait()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _cv.wait(lock, [=]
                 { return _count > 0; });
        --_count;
    }

    void Set(const int count)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _count = count;
    }

    int Count()
    {
        return _count;
    }

private:
    std::mutex _mutex;
    std::condition_variable _cv;
    int _count;
};

#endif