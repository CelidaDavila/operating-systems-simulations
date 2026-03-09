#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <iostream>
#include <queue>
#include <vector>
#include <thread>
#include <chrono>
#include <conio.h>
#include "Process.h"

#define SYSTEM_CAPACITY 5

#ifdef _WIN32
#define CLEAR "cls"
#elif defined(unix)||defined(__unix__)||defined(__unix)||defined(__APPLE__)||defined(__MACH__)
#define CLEAR "clear"
#else
#error "SO no soportado para limpiar pantalla"
#endif

using namespace std;
using namespace std::this_thread;
using namespace std::chrono;

class System{
private:
    queue<Process> newProcesses;
    queue<Process> readyProcesses;
    vector<Process> finishedProcesses;
    vector<Process> blockedProcesses;
    bool hasRunningProcess;
    Process runningProcess;
    int globalClock;
    int allProcessNumber;
public:
    System(){
        this->globalClock = 0;
        this->allProcessNumber = 0;
        this->hasRunningProcess = false;
    }
    void addNewProcess(Process &process){
        newProcesses.push(process);
        allProcessNumber++;
    }

    void addReadyProcesses(){
        while(getProcessesInMemory() < SYSTEM_CAPACITY && !newProcesses.empty()){
            Process process = newProcesses.front();
            newProcesses.pop();
            process.setArrivalTime(globalClock);
            process.setStatus("Ready");
            readyProcesses.push(process);
        }
    }

    void dispatchProcess(){
        if(!hasRunningProcess && !readyProcesses.empty()){
            runningProcess = readyProcesses.front();
            readyProcesses.pop();
            runningProcess.setStatus("Running");
            if(!runningProcess.getResponseRegistered()){
                runningProcess.setResponseTime(globalClock - runningProcess.getArrivalTime());
                runningProcess.setResponseRegistered(true);
            }

            hasRunningProcess = true;
        }
    }

    int getProcessesInMemory() {
        return readyProcesses.size() + blockedProcesses.size() + (hasRunningProcess ? 1 : 0);
    }

    void run(){
        addReadyProcesses();

        while(allProcessNumber > 0){
            addReadyProcesses();
            dispatchProcess();
            printData();
            sleep_for(seconds(1));
            if(_kbhit()){
                controlKbhit();
            }
            tick();
        }
    }

    void printData(){
        clearScreen();
        cout << "GLOBAL TIME: " << globalClock << endl;
        cout << "Number of processes (New state): " << newProcesses.size() << endl;
        printReadyProcesses();
        printRunningProcess();
        printFinishedProcesses();
        printBlockedProcesses();

    }


    void printReadyProcesses(){
        queue<Process> temporalQueue = readyProcesses;

        cout << "-------------------------------------" << endl;
        cout << "READY PROCESSES QUEUE" << endl;
        cout << "ID  EMT  ET\n";
        while(!temporalQueue.empty()){
            Process process = temporalQueue.front();
            cout << process.getId() << "   "<< process.getEstimatedTime() << "   "
                 << process.getElapsedTime() << endl;
            temporalQueue.pop();
        }
        cout << "-------------------------------------" << endl;
    }

    void printRunningProcess(){
        if(hasRunningProcess){
        cout << "RUNNING PROCESS" << endl;
        cout << "ID: " << runningProcess.getId() << endl;
        cout << "Elapsed Time: " << runningProcess.getElapsedTime() << endl;
        cout << "Estimated Time: " << runningProcess.getEstimatedTime() << endl;
        cout << "Remaining time: " << runningProcess.getRemainingTime() << endl;
        } else {
            cout << "RUNNING PROCESS: none" << endl;
        }
        cout << "-------------------------------------" << endl;
    }

    void printFinishedProcesses(){
        cout << "-------------------------------------" << endl;
        cout << "FINISHED PROCESSES QUEUE" << endl;
        cout << "ID    OPE        RES         State" << endl;
        for(Process& fp : finishedProcesses){
            cout << fp.getId() << "  ";
            if("Error" == fp.getStatus()){
                cout << fp.getOperand1() << " " << fp.getOperation() << " "
                << fp.getOperand2() << "                 " << fp.getStatus() << endl;
            }else{
                cout << fp.getOperand1() << " " << fp.getOperation() << " "
                << fp.getOperand2() <<"     "<< fp.getResult() <<  "        "
                << fp.getStatus() << endl;
            }
        }
        cout << "-------------------------------------" << endl;
    }

    void printBlockedProcesses(){
        cout << "-------------------------------------" << endl;
        cout << "BLOCKED PROCESSES" << endl;
        cout << "ID   Blocked Time Elapsed" << endl;

        for(Process& bp : blockedProcesses){
            cout << bp.getId() << "    " << bp.getBlockedTime() << endl;
        }

        cout << "-------------------------------------" << endl;
    }

    void tick(){
        globalClock++;
        updateBlockedProcesses();

        if(hasRunningProcess){
            if(runningProcess.getRemainingTime() == 0){
                if(runningProcess.getError()){
                    runningProcess.setStatus("Error");
                }else{
                    runningProcess.setStatus("Finished");
                }
                runningProcess.setFinishTime(globalClock);
                runningProcess.setReturnTime(runningProcess.getFinishTime() - runningProcess.getArrivalTime());
                runningProcess.setWaitingTime(runningProcess.getReturnTime() - runningProcess.getServiceTime());
                finishedProcesses.push_back(runningProcess);
                hasRunningProcess = false;
                allProcessNumber--;
            }else{
                runningProcess.setElapsedTime(runningProcess.getElapsedTime() + 1);
                runningProcess.setRemainingTime(runningProcess.getRemainingTime() - 1);
                runningProcess.setServiceTime(runningProcess.getServiceTime() + 1);
            }
        }
    }

    void controlKbhit(){
            char key = toupper(_getch());
            switch(key){
            case 'I':
                    {
                        if(hasRunningProcess){
                            runningProcess.setStatus("Blocked");
                            runningProcess.setBlockedTime(0);
                            blockedProcesses.push_back(runningProcess);
                            hasRunningProcess = false;

                        }
                    }
                break;
            case 'E':
                    {
                        runningProcess.setStatus("Error");
                        runningProcess.setRemainingTime(0);
                        runningProcess.setError(true);
                    }
                break;
            case 'P':
                    {
                    cout << "Process paused... Press C to continue";
                    char keyPressed = toupper(_getch());
                        while(keyPressed != 'C'){
                            keyPressed = toupper(_getch());
                        }
                    }
                break;
            default:
                break;
            }
    }

    void updateBlockedProcesses(){
        int i=0;
        while(i < blockedProcesses.size()){
            blockedProcesses[i].setBlockedTime(blockedProcesses[i].getBlockedTime() + 1);

            if(blockedProcesses[i].getBlockedTime() >= 8){
                blockedProcesses[i].setStatus("Ready");
                blockedProcesses[i].setBlockedTime(0);
                readyProcesses.push(blockedProcesses[i]);
                blockedProcesses.erase(blockedProcesses.begin() + i);
            } else {
                i++;
            }
        }
    }

    void printAllInfo(){
        clearScreen();
        cout << "ALL FINISHED PROCESSES" << endl;
        cout << "-------------------------------------" << endl;
        cout << "ID  ArrivalTime  FinishTime  ReturnTime  ResponseTime  WaitingTime  ServiceTime   State" << endl;
        for(Process& process : finishedProcesses){
            cout << process.getId() << "       " << process.getArrivalTime() << "            "
            << process.getFinishTime() << "            " << process.getReturnTime() << "            "
            << process.getResponseTime() << "            " << process.getWaitingTime()
            << "            " << process.getServiceTime() << "         "
            << process.getStatus() << endl;
        }
        cout << "-------------------------------------" << endl;
    }

    void clearScreen(){
        system(CLEAR);
    }

};

#endif // SYSTEM_H_INCLUDED
