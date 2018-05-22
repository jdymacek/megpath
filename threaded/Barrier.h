//https://stackoverflow.com/questions/24465533/implementing-boostbarrier-in-c11
#ifndef BARRIER_H
#define BARRIER_H

#include <mutex>
#include <condition_variable>

using namespace std;

class Barrier {
public:
    explicit Barrier(std::size_t iCount) : 
      mThreshold(iCount), 
      mCount(iCount), 
      mGeneration(0) {
    }

    void Wait() {
        unique_lock<std::mutex> lLock{mMutex};
        auto lGen = mGeneration;
        if (!--mCount) {
            mGeneration++;
            mCount = mThreshold;
            mCond.notify_all();
        } else {
            mCond.wait(lLock, [this, lGen] { return lGen != mGeneration; });
        }
    }

private:
	mutex mMutex;
    condition_variable mCond;
    size_t mThreshold;
    size_t mCount;
    size_t mGeneration;
};
#endif
