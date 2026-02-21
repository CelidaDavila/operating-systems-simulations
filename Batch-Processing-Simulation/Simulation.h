#ifndef SIMULATION_H_INCLUDED
#define SIMULATION_H_INCLUDED

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <deque>
#include <thread>
#include <chrono>
#include "Batch.h"

using namespace std;
using namespace std::this_thread;
using namespace std::chrono;

class Simulation{
private:
    vector<Batch> batches;
    int globalTime;
public:
    Simulation(){
        this->globalTime=0;
    }
    void addProcess(Process &process){
        if (batches.empty() || batches.back().isFull()){
        batches.push_back(Batch());
        }
        batches.back().addProcess(process);
    }

    void printData(){
        int batchesCont = 0;
        int processIndex;
        string printBatch,printProcess;
        int const totalBatches = batches.size();
        vector<string> finishedProcesses = printFinishedProcesses();

        while(batchesCont < totalBatches){
            int pendingBatchesNumber = totalBatches - batchesCont - 1;
            Batch& actualBatch = batches[batchesCont];

            if(actualBatch.hasReadyProcess()){
                actualBatch.cleanRunningProcessIndex();
                actualBatch.startNextProcess();
                processIndex = actualBatch.getRunningProcessIndex();
                Process& actualProcess = actualBatch.getProcess(processIndex);

                while (actualProcess.getRemainingTime() > 0){
                    clearScreen();
                    printBatch = formatActualBatch(actualBatch);
                    printProcess = formatActualProcess(actualProcess);
                    renderScreenBlocks(pendingBatchesNumber,globalTime,printBatch, printProcess,finishedProcesses);
                    sleep_for(seconds(1));
                    globalTime++;
                    actualProcess.tickOneSecond();
                }

                finishedProcesses.push_back(to_string(actualProcess.getId())+"     "+
                                        to_string(actualProcess.getOperand1())+" "+
                                        string(1,actualProcess.getOperation())+" "+
                                        to_string(actualProcess.getOperand2())+"     "+
                                        actualProcess.getResult()+"     "+
                                        to_string(batchesCont+1));
                clearScreen();
                //last print
                printBatch = formatActualBatch(actualBatch);
                printProcess = formatActualProcess(actualProcess);
                renderScreenBlocks(pendingBatchesNumber,globalTime,printBatch, printProcess,finishedProcesses);

            }else{
                batchesCont++;
            }
        }
    }

    string formatActualBatch(Batch& batch){
        const deque<int>& readyDeque = batch.getReadyDeque();
        ostringstream oss;
        oss << "Current Batch\n";
        oss << "Name           EMT\n";

        for (int i : readyDeque){
            Process& process = batch.getProcess(i);

            oss << process.getName() << "    " << process.getEstimatedTime() << "\n";
        }
        return oss.str();
    }


    string formatActualProcess(Process& process){
        ostringstream oss;
        oss << "Actual Process\n";
        oss << "Name: " << process.getName() << "\n";
        oss << "Ope:  " << process.getOperand1() << " " << process.getOperation() << " "
        << process.getOperand2() << "\n";
        oss << "EMT:  " << process.getEstimatedTime() << " s\n";
        oss << "ID:   " << process.getId() << "\n";
        oss << "ET:   " << process.getElapsedTime() << " s\n";
        oss << "RT:   " << process.getRemainingTime() << " s\n";
        return oss.str();
    }

    vector<string> printFinishedProcesses(){
        vector<string>finishedProcesses;
        finishedProcesses.push_back("Finished Processes");
        finishedProcesses.push_back("ID    OPE        RES   BatchNumber");
        return finishedProcesses;
    }

    void renderScreenBlocks(int pendingBatchesNumber, int globalTime,const string& actualBatch,
                            const string& actualProcess,const vector<string>& finishedProcesses)
    {
        cout << "---------------------------------------------------------" << endl;
        cout << "Number of remaining batches: " << pendingBatchesNumber << endl;
        cout << "Global time: " << globalTime << " s\n";
        cout << "---------------------------------------------------------" << endl;
        cout << actualBatch << "\n";
        cout << "---------------------------------------------------------" << endl;
        cout << actualProcess << "\n";
        cout << "---------------------------------------------------------" << endl;

        for (const string& line : finishedProcesses){
            cout << line << endl;
        }

        cout << "---------------------------------------------------------" << endl;
    }


    void clearScreen(){
        cout << "\033[2J\033[H";
    }

};

#endif // SIMULATION_H_INCLUDED
