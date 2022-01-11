#ifndef EXERCISES_PETERSONLOCK_H
#define EXERCISES_PETERSONLOCK_H

#include <memory>
#include <atomic>


class PetersonLock
{
private:
    //indexed by thread ID, 0 or 1
    std::atomic<bool> interested[2];
    //who’s yielding priority?
    std::atomic<int> turn;

public:
    PetersonLock()
    {
        turn.store(0, std::memory_order_release);
        interested[0].store(false, std::memory_order_release);
        interested[1].store(false, std::memory_order_release);
    }

    /*
     * id from this thread muss be handed in
     */
    void lock(int threadID)
    {
        int me = threadID;      //either 0 or 1
        int he = 1 - me;        //the other thread
        interested[me].exchange(true, std::memory_order_acq_rel);
        turn.store(me, std::memory_order_release);
        while (interested[he].load(std::memory_order_acquire) && turn.load(std::memory_order_acquire) == me)
            continue; //spin
    }

    /*
     * id from this thread muss be handed in
     */
    void unlock(int threadID)
    {
        int me = threadID;
        interested[me].store(false, std::memory_order_release);
    }
};

#endif //EXERCISES_PETERSONLOCK_H
