#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <map>
#include <algorithm>
#include <tuple> 

using namespace std;

int main()
{
    cout<<"Hello World" << endl;

    std::thread thread_instance([] {
        cout << "Inside Thread" << endl;
        // std::this_thread::sleep_for(std::chrono::seconds(1));
    });


    std::thread thread_instance1([] {
        cout << "Inside Thread 1" << endl;
        // std::this_thread::sleep_for(std::chrono::seconds(1));
    });


    std::thread thread_instance2([] {
        cout << "Inside Thread 2" << endl;
      //  std::this_thread::sleep_for(std::chrono::seconds(1));
    });

    thread_instance.detach();
    thread_instance1.detach();
    thread_instance2.detach();
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // Check for_each
    std::vector<int> vec = {1,2,3,4,5};
    int sum = 0;
    std::for_each(vec.begin(), vec.end(), [&] (int a) { sum += a;});

    cout << sum << endl;
    

    return 0;
}

