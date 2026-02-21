#ifndef BATCH_H_INCLUDED
#define BATCH_H_INCLUDED

#include "Process.h"
#include <vector>
#include <deque>

#define BATCH_CAPACITY 5

using namespace std;

class Batch{
private:
    vector<Process> processes;
    deque<int> ready;
    int runningProcessIndex;
public:
    Batch(){
        this->runningProcessIndex = -1;
    }
    void addProcess(Process &process){
        processes.push_back(process);
        ready.push_back(processes.size()-1);
    }
    bool isFull() const{
        return processes.size() >= BATCH_CAPACITY;
    }
    Process& getProcess(int index){
        return processes[index];
    }

    deque<int>& getReadyDeque(){
        return ready;
    }
    void startNextProcess() {
    runningProcessIndex = ready.front();
    ready.pop_front();
    }
    int getRunningProcessIndex(){
        return runningProcessIndex;
    }
    void cleanRunningProcessIndex(){
        runningProcessIndex = -1;
    }
    bool hasReadyProcess() const {
        return !ready.empty();
    }
};

#endif // BATCH_H_INCLUDED
