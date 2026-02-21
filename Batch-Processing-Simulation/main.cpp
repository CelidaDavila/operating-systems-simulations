//Celida Davila

#include <iostream>
#include <vector>
#include <unordered_set>
#include <deque>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <algorithm>

//Constant
#define BATCH_CAPACITY 5

using namespace std;
using namespace std::this_thread;
using namespace std::chrono;


class Process{
private:
    string programmerName;
    char operation;
    int operand1,operand2;
    int estimatedTime;
    int remainingTime;
    int elapsedTime;
    int id;

public:
    Process(string programmerName,char operation,int operand1, int operand2,int estimatedTime,int id){
        this-> programmerName = programmerName;
        this-> operation = operation;
        this-> operand1= operand1;
        this-> operand2 = operand2;
        this-> estimatedTime = estimatedTime;
        this-> remainingTime = estimatedTime;
        this-> elapsedTime = 0;
        this-> id = id;
    }

    string getResult() const {
        double result=0;
        switch(operation){
        case '+':
            result = operand1 + operand2;
            break;
        case '-':
            result = operand1 - operand2;
            break;
        case '*':
            result = operand1 * operand2;
            break;
        case '/':
            result = (double)operand1 / operand2;
            break;
        case '%':
            result = operand1 % operand2;
            break;
        case '^':
            result = pow(operand1,operand2);
            break;
        default:
            break;
        }
        ostringstream oss;
        oss << fixed << setprecision(2) << result;
        return oss.str();
    }

    string getName() const{
        return programmerName;
    }
    char getOperation() const{
        return operation;
    }
    int getOperand1() const{
        return operand1;
    }
    int getOperand2() const{
        return operand2;
    }
    int getEstimatedTime()const{
        return estimatedTime;
    }
    int getRemainingTime() const{
        return remainingTime;
    }
    int getElapsedTime() const{
        return elapsedTime;
    }
    int getId() const{
        return id;
    }
    void tickOneSecond(){
        if(remainingTime > 0){
            remainingTime--;
            elapsedTime++;
        }
    }
};

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
        ready.push_back((int)processes.size()-1);
    }
    bool isFull() const{
        return processes.size() >= BATCH_CAPACITY;
    }
    int processesCount(){
        return processes.size();
    }
    Process& getProcess(int index){
        return processes[index];
    }
    int getRunningProcessIndex(){
        return runningProcessIndex;
    }
    void cleanRunningProcessIndex(){
        runningProcessIndex = -1;
    }

    deque<int>& getReadyDeque(){
        return ready;
    }
    bool hasReadyProcess() const {
        return !ready.empty();
    }
    void startNextProcess() {
    runningProcessIndex = ready.front();
    ready.pop_front();
    }
};

vector<Batch> batches;

void registerProcesses(int numProcesses);
void createProcess(string name,char operation,int operand1,int operand2,int estimatedTime,int id);
char validateOperation(char operation);
int validateSecondOperand(int operand2,char operation);
int validateEstimatedTime(int estimatedTime);
void printData();
string formatActualBatch(Batch& batch);
string formatActualProcess(Process& process);
vector<string> printFinishedProcesses();
void renderScreenBlocks(int pendingBatchesNumber, int globalTime,const string& actualBatch,
                        const string& actualProcess,const vector<string>& finishedProcesses);
void clearScreen();

int main()
{
    int numProcesses;
    cout << "How many processes do you want to create?: ";
    cin >> numProcesses;
    cin.get();
    registerProcesses(numProcesses);
    printData();
    cout << "Press ENTER to finish...." << endl;
    cin.get();

    return 0;
}


void registerProcesses(int numProcesses){
    string name;
    char operation;
    int cont = 0,estimatedTime,operand1,operand2,id;
    unordered_set<int>usedId;

    while(cont < numProcesses){
        cout << "\nProcess " << cont+1 << endl;
        cout << "Programmer name:";
        getline(cin,name);
        cout << "Operation to perform (+,-,*,/,%,^): ";
        cin >> operation;
        operation = validateOperation(operation);
        cout << "Operand 1 (int): ";
        cin >> operand1;
        cout << "Operand 2 (int): ";
        cin >> operand2;
        operand2 = validateSecondOperand(operand2,operation);
        cout << "Estimated maximum time for this process (seconds, int): ";
        cin >> estimatedTime;
        estimatedTime = validateEstimatedTime(estimatedTime);
        cout << "Program number (ID int): ";
        cin >> id;

        //Validate id/process number using a hash set
        while (usedId.count(id)){
            cout << "That id was already used" << endl;
            cout << "Program number (ID int): ";
            cin >> id;
        }
        usedId.insert(id);

        cin.get();
        createProcess(name,operation,operand1,operand2,estimatedTime,id);
        cont++;
    }
}


void createProcess(string name,char operation,int operand1,int operand2,int estimatedTime,int id){
    Process process(name,operation,operand1,operand2,estimatedTime,id);

    if (batches.empty() || batches.back().isFull()){
        batches.push_back(Batch());
    }
    batches.back().addProcess(process);
}

char validateOperation(char operation){
    char newOperation = operation;
    while (newOperation != '+' && newOperation != '-' && newOperation != '*'
           && newOperation != '/' && newOperation != '%' && newOperation != '^'){
            cout << "Invalid operation. Please try again" << endl;
            cout << "Operation to perform (+,-,*,/,%,^): ";
            cin >> newOperation;
    }
    return newOperation;
}

int validateSecondOperand(int operand2,char operation){
    int newOperand = operand2;
    while ((operation == '%' || operation == '/') && newOperand == 0){
        cout << "Operand 2 cannot be 0 for the " << operation << " operation" << endl;
        cout << "Operand 2 (int): ";
        cin >> newOperand;
    }
    return newOperand;
}

int validateEstimatedTime(int estimatedTime){
    int newEstimatedTime = estimatedTime;
    while (newEstimatedTime <= 0){
        cout << "Time cannot be zero or less" << endl;
        cout << "Estimated maximum time for this process (seconds, int): ";
        cin >> newEstimatedTime;
    }
    return newEstimatedTime;
}

void printData(){
    int globalTime = 0;
    int batchesCont = 0;
    int processIndex = 0;
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
