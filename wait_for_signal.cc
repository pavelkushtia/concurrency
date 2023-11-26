#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

/**
* Problem Statement 
* --------------------
* An event is planned to occur that sets a flag. 
* There are N number of tasks that happens, k of which happen before the flag is
* set. Rest of the tasks happen after the flag set. The tasks happens before The
* flag is set should wait for the flag to get set. The tasks after the flag set
* can executes immediately. 
* Event can happen independently and concurrently. 
**/

/**
* Solution Approach 
* ---------------------
* Condition varianble with std::thread based solution.
**/

std::mutex mutex_;
std::condition_variable check_flag_set_condition_;
bool flag_ = false;


void ThreadFunction(int thread_id) {
    std::unique_lock lock(mutex_);
    check_flag_set_condition_.wait(lock, [] () {return flag_;} );    
    std::cout << "Finished: " << thread_id << std::endl;
}

void TriggerFunction() {
    std::unique_lock lock(mutex_);
    flag_ = true;
    lock.unlock();
    
    check_flag_set_condition_.notify_all();
}


int main()
{
    int thread_index = 0;
    int n = 10;
    int k = 6;
    std::vector<std::thread> threads;
    while ( thread_index < k ) {
        threads.push_back(std::thread(&ThreadFunction, thread_index++));
    }
    
    auto trigger_thread = std::thread(&TriggerFunction);
    
    while ( thread_index < n ) {
        threads.push_back(std::thread(&ThreadFunction, thread_index++));
    }
    
    trigger_thread.join();
    for ( auto& thread : threads ) {
        thread.join();
    }

    trigger_thread.join();
    
    return 0;
}
