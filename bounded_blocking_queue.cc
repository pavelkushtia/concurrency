/******************************************************************************

A simple bounded ( limited capacity ) queue. The operations are blocked.

 - Extract is blocked if the queue is empty, and wait until an element is avaiable.
 - Put is blocked if the capacity is full and wait until an element is extracted. 
 - Put supports atomic insertion of multiple elements. 

*******************************************************************************/
#include <iostream>
#include <list>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

template <typename T>
class BoundedBlockingQueue {
  public: 
    BoundedBlockingQueue(int capacity) {
        capacity_ = capacity;
    }
  
    void Put(const T& value) {
        std::unique_lock lock(mutex_);
        queue_availability_condition_variable_.wait(lock, [this] { return queue_.size() < capacity_; });
        
        queue_.push(value);

        lock.unlock();
        queue_availability_condition_variable_.notify_one();
    }
    
    void Put(const std::list<T>& values) {
        std::unique_lock lock(mutex_);
        
        for (auto value : values ) {
            queue_availability_condition_variable_.wait(lock, [this] { return queue_.size() < capacity_; });
            queue_.push(value);
        }

        lock.unlock();
        queue_availability_condition_variable_.notify_all();
    }
    
    T Extract() {
        std::unique_lock lock(mutex_);
        queue_availability_condition_variable_.wait(lock, [this] { return !queue_.empty(); });
        
        T value = queue_.front();
        queue_.pop();
        
        lock.unlock();
        queue_availability_condition_variable_.notify_one();
        
        return value;
    }
    
    size_t Size() {
        return queue_.size();
    }
  
  private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable queue_availability_condition_variable_;
    int capacity_;
};


using namespace std;

int main()
{
    BoundedBlockingQueue<int> bounded_blocking_queue_(3);
    std::thread thread_1( [&] { bounded_blocking_queue_.Put(1); } );
    
    thread_1.join();
    cout<< "Queue Size - First Attempt: " << bounded_blocking_queue_.Size() << endl;
    
    std::thread thread_2( [&] { bounded_blocking_queue_.Extract(); } );
    std::thread thread_3( [&] { bounded_blocking_queue_.Extract(); } );

    std::thread thread_4( [&] { bounded_blocking_queue_.Put( {1,2,4,5,6} ); } );
    std::thread thread_5( [&] { bounded_blocking_queue_.Extract(); } );

    thread_2.join();
    thread_3.join();
    thread_4.join();
    thread_5.join();
    
    cout<< "Queue Size - Second Attempt: " << bounded_blocking_queue_.Size() << endl;

    std::thread thread_6( [&] { bounded_blocking_queue_.Put(7); } );
    thread_6.join();

    cout<< "Queue Size - Third Attempt: " << bounded_blocking_queue_.Size() << endl;

    return 0;
}
