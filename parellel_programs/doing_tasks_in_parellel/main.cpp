#include <queue>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>
#include <iostream>
/*
status has value 1|2|3
1. can be computed for more then one task
2. can be computed singly
3. can be computed for more then one task
*/ 
class Task {
    private:
        const std::string name;
    public:
        int status;
        Task(std::string name) : status(0), name(name) {}
        ~Task() = default;
        std::string getName() {
            return name;
        }
};

std::mutex m1;
std::chrono::milliseconds s3000(3000);
std::chrono::milliseconds s5000(5000);
std::chrono::milliseconds s1000(1000);

std::queue<Task*> processed1;
std::queue<Task*> processed2;
std::queue<Task*> processed3;

std::atomic<bool> running = false;


void logging(const std::string& logMsg) {
    std::cout<<logMsg<<std::endl;
}


void process_1(Task* task) {
    logging("1 running for " + task->getName() + " ");
    std::this_thread::sleep_for(s3000);
    processed1.push(task);
    return;
}

void process_2(Task* task) {
    m1.lock();
    logging("2 running for " + task->getName() + " ");
    std::this_thread::sleep_for(s1000);
    m1.unlock();
    processed2.push(task);
}

void process_3(Task* task) {
    logging("3 running for " + task->getName() + " ");
    std::this_thread::sleep_for(s5000);
    processed3.push(task);
    return;
}

void process(Task* task) {
    // while (!running) {
        if (task->status == 0) {
            std::thread t1(process_1, task);
            t1.detach();
            task->status = 1;
        } else if(task->status == 1) {
            std::thread t2(process_2, task);
            t2.detach();
            task->status = 2;
        } else if(task->status == 2) {
            std::thread t3(process_3, task);
            t3.detach();
            task->status = 3;
        }
    // }
}

void startTask(std::queue<Task*>& workkQueue) {
    while(1) {
        if (!workkQueue.empty()) {
            Task* task = workkQueue.front();
            process(task);
            workkQueue.pop();
        }

        if (!processed1.empty()) {
            Task* task = processed1.front();
            process(task);
            processed1.pop();
        }

        if (!processed2.empty()) {
            Task* task = processed2.front();
            process(task);
            processed2.pop();
        }

        if (!processed3.empty()) {
            Task* task = processed3.front();
            process(task);
            processed3.pop();
        }
    }
}


int main() {
    std::queue<Task*> workQueue;
    workQueue.push(new Task("Task_1"));
    workQueue.push(new Task("Task_2"));
    workQueue.push(new Task("Task_3"));
    workQueue.push(new Task("Task_4"));

    startTask(workQueue);
}