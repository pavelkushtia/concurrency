#include <condition_variable>
#include <iostream>
#include <mutex>
#include <future>
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
* Condition varianble with std::async based solution.
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
    std::vector<std::future<void>> futures;
    while ( thread_index < k ) {
        futures.push_back(std::async(std::launch::async, &ThreadFunction, thread_index++));
    }
    
    auto trigger_future = std::async(std::launch::async, &TriggerFunction);
    
    while ( thread_index < n ) {
        futures.push_back(std::async(std::launch::async, &ThreadFunction, thread_index++));
    }
    
    trigger_future.get();
    for ( auto& future : futures ) {
        future.get();
    }
    
    return 0;
}
