#ifndef SIMULATION_H_INCLUDED
#define SIMULATION_H_INCLUDED

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <deque>
#include <thread>
#include <chrono>
#include <conio.h>
#include "Batch.h"

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

class Simulation{
private:
    vector<Batch> batches;
    int globalTime;
    struct FinishedProcess{
        int id;
        char operation;
        int operand1;
        int operand2;
        string result;
        string state;
        bool error;
        int batchNumber;
    };
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
        vector<FinishedProcess> finishedProcesses;

        while(batchesCont < totalBatches){
            int pendingBatchesNumber = totalBatches - batchesCont - 1;
            Batch& actualBatch = batches[batchesCont];

            if(actualBatch.hasReadyProcess()){
                actualBatch.cleanRunningProcessIndex();
                actualBatch.startNextProcess();
                processIndex = actualBatch.getRunningProcessIndex();


                while (actualBatch.getProcess(processIndex).getRemainingTime() > 0){
                        Process& actualProcess = actualBatch.getProcess(processIndex);

                        clearScreen();
                        printBatch = formatActualBatch(actualBatch);
                        printProcess = formatActualProcess(actualProcess);
                        renderScreenBlocks(pendingBatchesNumber,globalTime,printBatch, printProcess,finishedProcesses);
                        sleep_for(seconds(1));
                        globalTime++;
                        actualProcess.tickOneSecond();
                        if(_kbhit()){
                            bool changedProcess = controlKbhit(actualBatch,processIndex);
                            if(changedProcess){
                                processIndex = actualBatch.getRunningProcessIndex();
                            }
                        }
                }
                Process& processFinished = actualBatch.getProcess(processIndex);
                FinishedProcess fp;
                fp.id = processFinished.getId();
                fp.operand1 = processFinished.getOperand1();
                fp.operand2 = processFinished.getOperand2();
                fp.operation = processFinished.getOperation();
                fp.result = processFinished.getResult();
                fp.state = processFinished.getStatus();
                fp.error = processFinished.getError();
                fp.batchNumber = batchesCont+1;
                finishedProcesses.push_back(fp);

                clearScreen();
                //last print
                printBatch = formatActualBatch(actualBatch);
                printProcess = formatActualProcess(processFinished);
                renderScreenBlocks(pendingBatchesNumber,globalTime,printBatch, printProcess,finishedProcesses);

            }else{
                batchesCont++;
            }
        }
    }

    string formatActualBatch(Batch& batch){
        const deque<int>& readyDeque = batch.getReadyDeque();
        ostringstream oss;
        oss << "Current Batch\n"; oss << left << setw(6) << "ID" << setw(8) << "EMT"
        << setw(8) << "ET" << "\n";

        for (int i : readyDeque){
            Process& process = batch.getProcess(i);

            oss << left
                << setw(6) << process.getId()
                << setw(8) << (to_string(process.getEstimatedTime()) + "s")
                << setw(8) << (to_string(process.getElapsedTime()) + "s")
                << "\n";
        }
        return oss.str();
    }


    string formatActualProcess(Process& process){
        ostringstream oss;
        oss << "Actual Process\n";
        oss << left << setw(6) << "ID:"  << process.getId() << "\n";
        oss << left << setw(6) << "Ope:" << process.getOperand1() << " "
            << process.getOperation() << " " << process.getOperand2() << "\n";
        oss << left << setw(6) << "EMT:" << process.getEstimatedTime() << " s\n";
        oss << left << setw(6) << "ET:"  << process.getElapsedTime() << " s\n";
        oss << left << setw(6) << "RT:"  << process.getRemainingTime() << " s\n";

        return oss.str();
    }

    void renderScreenBlocks(int pendingBatchesNumber, int globalTime,const string& actualBatch,
                            const string& actualProcess,const vector<FinishedProcess>& finishedProcesses)
    {
        cout << "'I' = interrumpt 'E' = error 'P' = pause 'C'= continue " << endl;
        cout << "---------------------------------------------------------" << endl;
        cout << "Number of remaining batches: " << pendingBatchesNumber << endl;
        cout << "Global time: " << globalTime << " s\n";
        cout << "---------------------------------------------------------" << endl;
        cout << actualBatch << "\n";
        cout << "---------------------------------------------------------" << endl;
        cout << actualProcess << "\n";
        cout << "---------------------------------------------------------" << endl;
        cout << "Finished Processes" << endl;
        cout << left << setw(6)  << "ID" << setw(16) << "OPE" << setw(12) << "RES"
             << setw(14) << "BatchNumber" << setw(12) << "State" << endl;

        for (const FinishedProcess& fp : finishedProcesses){
            if (fp.error){
                cout << left << setw(6)  << fp.id << setw(16) << "ERROR" << setw(12) << "-----"
                     << setw(14) << fp.batchNumber << setw(12) << fp.state << endl;
            } else {
                string operation = to_string(fp.operand1) + " " +
                                   string(1, fp.operation) + " " +
                                   to_string(fp.operand2);

                cout << left << setw(6)  << fp.id << setw(16) << operation
                     << setw(12) << fp.result << setw(14) << fp.batchNumber << setw(12)
                     << fp.state << endl;
            }
        }
        cout << "---------------------------------------------------------" << endl;
    }

    bool controlKbhit(Batch& actualBatch,int processIndex){
            char key = toupper(_getch());
            switch(key){
            case 'I':
                    {
                        actualBatch.interrumpProcess();
                        return true;
                    }
                break;
            case 'E':
                    {
                        Process& actualProcess = actualBatch.getProcess(processIndex);
                        actualProcess.setStatus("Error");
                        actualProcess.setRemainingTime(0);
                        actualProcess.setError();
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
        return false;
    }

    void clearScreen(){
        system("cls");
    }

};

#endif // SIMULATION_H_INCLUDED
