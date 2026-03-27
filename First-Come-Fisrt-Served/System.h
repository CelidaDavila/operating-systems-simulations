#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <iostream>
#include <iomanip>
#include <queue>
#include <vector>
#include <thread>
#include <chrono>
#include <conio.h>
#include "Process.h"
#include "Util.h"

#define SYSTEM_CAPACITY 5

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
    int id;
public:
    System(){
        this->globalClock = 0;
        this->allProcessNumber = 0;
        this->hasRunningProcess = false;
        this->id=0;
        srand((unsigned)time(nullptr));
    }

    void registerProcesses(int numProcesses){
        char operations[] = {'+', '-', '*', '/', '%', '^'};
        char operation;
        int cont = 0,estimatedTime,operand1,operand2;

        while(cont < numProcesses){
            id++;
            operation = operations[rand() % 6];
            operand1 = randInRange(-100, 100);
            operand2 = randInRange(-100, 100);
            operand2 = validateSecondOperand(operand2,operation);
            estimatedTime = randInRange(6, 20);

            createProcess(operation,operand1,operand2,estimatedTime,id);
            cont++;
        }
    }

    int randInRange(int lowBound, int highBound) {
        return lowBound + (rand() % (highBound - lowBound + 1));
    }

    void createProcess(char operation,int operand1,int operand2,int estimatedTime,int id){

        Process process(operation,operand1,operand2,estimatedTime,id);
        addNewProcess(process);

    }

    int validateSecondOperand(int operand2,char operation){
        if ((operation == '/' || operation == '%') && operand2 == 0){
            operand2 = randInRange(-100, 100);
        }
        return operand2;
    }


    void startProgram(){
        int numProcesses;
        cout << "How many processes do you want to create?: ";
        cin >> numProcesses;
        cin.get();
        registerProcesses(numProcesses);
        run();
        cout << "Press ENTER to list all process info...." << endl;
        cin.get();
        printAllInfo();
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
        cout << "'I' = interrumpt 'E' = error 'P' = pause 'C'= continue 'N' = new 'B' = processes table " << endl;
        cout << "GLOBAL TIME: " << globalClock << endl;
        cout << "Number of processes (New state): " << newProcesses.size() << endl;
        printReadyProcesses();
        printRunningProcess();
        printFinishedProcesses();
        printBlockedProcesses();

    }


    void printReadyProcesses(){
        queue<Process> temporalQueue = readyProcesses;

        printSmallSeparator();
        cout << "READY PROCESSES QUEUE" << endl;
        cout << left << setw(8)  << "ID" << setw(12) << "EMT"<< setw(12) << "ET" << endl;
        while(!temporalQueue.empty()){
            Process process = temporalQueue.front();
            cout << left << setw(8)  << process.getId() << setw(12) << process.getEstimatedTime()
             << setw(12) << process.getElapsedTime() << endl;
            temporalQueue.pop();
        }
        printSmallSeparator();
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
    }

    void printFinishedProcesses(){
        printSmallSeparator();
        cout << "FINISHED PROCESSES QUEUE" << endl;
        cout << left << setw(8)  << "ID" << setw(18) << "OPERATION" << setw(12) << "RESULT"
         << setw(12) << "STATE" << endl;

        for(Process& fp : finishedProcesses){
            stringstream op, res;

            op << fp.getOperand1() << " " << fp.getOperation() << " " << fp.getOperand2();

            if (fp.getStatus() == "Error") {
                res << "-";
            } else {
                res << fp.getResult();
            }

            cout << left << setw(8)  << fp.getId() << setw(20) << op.str() << setw(12) << res.str()
                 << setw(12) << fp.getStatus() << endl;
        }
    }

    void printBlockedProcesses(){
        printSmallSeparator();
        cout << "BLOCKED PROCESSES" << endl;
        cout << left << setw(8)  << "ID" << setw(20) << "BLOCKED TIME" << endl;

        for (Process& bp : blockedProcesses) {
            cout << left << setw(8)  << bp.getId() << setw(20) << bp.getBlockedTime() << endl;
            }

        printSmallSeparator();
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
            case 'N':
                    {
                       registerProcesses(1);
                    }
                break;
            case 'B':
                    {
                        printInfoButtonB();
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
        while(i < (int)blockedProcesses.size()){
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
        printLongSeparator();
        cout << left << setw(8)  << "ID" << setw(10) << "Arrival" << setw(10) << "Finish"
            << setw(14) << "Return" << setw(14) << "Response" << setw(10) << "Waiting"
            << setw(10) << "Service" << setw(12) << "State" << endl;

        printLongSeparator();

        for (Process& process : finishedProcesses) {
            cout << left << setw(8)  << process.getId() << setw(10) << process.getArrivalTime()
                 << setw(10) << process.getFinishTime() << setw(14) << process.getReturnTime()
                 << setw(14) << process.getResponseTime() << setw(10) << process.getWaitingTime()
                 << setw(10) << process.getServiceTime() << setw(12) << process.getStatus() << endl;
        }
        printLongSeparator();
    }

    void printInfoButtonB(){
        clearScreen();
        cout << "ALL PROCESSES INFO" << endl;
        printLongSeparator();
        cout << left << setw(8)  << "ID" << setw(10) << "Arrival" << setw(10) << "Finish"
            << setw(14) << "Return" << setw(14) << "Response" << setw(10) << "Waiting"
            << setw(10) << "Service" << setw(12) << "State" << endl;

        printLongSeparator();
        queue<Process> tempNew = newProcesses;
        while(!tempNew.empty()){
            Process process = tempNew.front();
            tempNew.pop();
            cout << left << setw(8) << process.getId()
                 << setw(10) << process.getArrivalTime()
                 << setw(10) << process.getFinishTime()
                 << setw(14) << process.getReturnTime()
                 << setw(14) << process.getResponseTime()
                 << setw(10) << process.getWaitingTime()
                 << setw(10) << process.getServiceTime()
                 << setw(12) << process.getStatus() << endl;
        }

        queue<Process> tempReady = readyProcesses;
        while(!tempReady.empty()){
            Process process = tempReady.front();
            tempReady.pop();
            cout << left << setw(8) << process.getId()
                 << setw(10) << process.getArrivalTime()
                 << setw(10) << process.getFinishTime()
                 << setw(14) << process.getReturnTime()
                 << setw(14) << process.getResponseTime()
                 << setw(10) << process.getWaitingTime()
                 << setw(10) << process.getServiceTime()
                 << setw(12) << process.getStatus() << endl;
        }

        if(hasRunningProcess){
            cout << left << setw(8) << runningProcess.getId()
                 << setw(10) << runningProcess.getArrivalTime()
                 << setw(10) << runningProcess.getFinishTime()
                 << setw(14) << runningProcess.getReturnTime()
                 << setw(14) << runningProcess.getResponseTime()
                 << setw(10) << runningProcess.getWaitingTime()
                 << setw(10) << runningProcess.getServiceTime()
                 << setw(12) << runningProcess.getStatus() << endl;
        }

        for (Process& process : blockedProcesses) {
            cout << left << setw(8)  << process.getId() << setw(10) << process.getArrivalTime()
                 << setw(10) << process.getFinishTime() << setw(14) << process.getReturnTime()
                 << setw(14) << process.getResponseTime() << setw(10) << process.getWaitingTime()
                 << setw(10) << process.getServiceTime() << setw(12) << process.getStatus() << endl;
        }

        for (Process& process : finishedProcesses) {
            cout << left << setw(8)  << process.getId() << setw(10) << process.getArrivalTime()
                 << setw(10) << process.getFinishTime() << setw(14) << process.getReturnTime()
                 << setw(14) << process.getResponseTime() << setw(10) << process.getWaitingTime()
                 << setw(10) << process.getServiceTime() << setw(12) << process.getStatus() << endl;
        }
        printLongSeparator();
    }


};



#endif // SYSTEM_H_INCLUDED
