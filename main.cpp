//Celida Davila

#include <iostream>
#include <vector>
#include <unordered_set>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <algorithm>

//Constant
#define BATCH_CAPACITY 4

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

vector<vector<Process>> batches;

void registerProcesses(int numProcesses);
void createProcess(string name,char operation,int operand1,int operand2,int estimatedTime,int id);
char validateOperation(char operation);
int validateSecondOperand(int operand2,char operation);
int validateEstimatedTime(int estimatedTime);
void printData();
vector<string> printActualBatch(int batchesCont);
vector<string> printActualProcess(int batchesCont, int processCont);
vector<string> printFinishedProcesses();
void renderScreen(int pendingBatchesNumber,int globalTime,const vector<string> &actualBatch,
                  const vector<string> &actualProcess, const vector<string> &finishedProcesses);
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

/*This function creates a new "process" object each time it is called by the function "registerProcesses()"
and it is appended to the last batch, if there are no batches yet,
or the last batch already contains 4 processes, it creates a new batch first*/

void createProcess(string name,char operation,int operand1,int operand2,int estimatedTime,int id){
    Process process(name,operation,operand1,operand2,estimatedTime,id);
    if (batches.empty() || batches.back().size() == BATCH_CAPACITY){
        batches.push_back(vector<Process>());
    }
    batches.back().push_back(process);
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
    int processCont = 0;
    int const totalBatches = batches.size();
    vector<string>actualBatch;
    vector<string>actualProcess;
    vector<string>finishedProcesses;

    finishedProcesses = printFinishedProcesses(); //just the two first lines

    while (batchesCont < totalBatches){
        int pendingBatchesNumber = totalBatches - batchesCont - 1;
        Process& thisProcess = batches[batchesCont][processCont];
        actualBatch = printActualBatch(batchesCont);

        while (thisProcess.getRemainingTime() > 0){
            actualProcess = printActualProcess(batchesCont,processCont);
            clearScreen();
            renderScreen(pendingBatchesNumber, globalTime, actualBatch, actualProcess, finishedProcesses);
            sleep_for(seconds(1));
            globalTime++;
            thisProcess.tickOneSecond();

        }
        finishedProcesses.push_back(to_string(batches[batchesCont][processCont].getId())+"     "+
                                    to_string(batches[batchesCont][processCont].getOperand1())+" "+
                                    string(1,batches[batchesCont][processCont].getOperation())+" "+
                                    to_string(batches[batchesCont][processCont].getOperand2())+"     "+
                                    batches[batchesCont][processCont].getResult()+"     "+
                                    to_string(batchesCont+1));
        clearScreen();
        actualProcess = printActualProcess(batchesCont, processCont);
        renderScreen(pendingBatchesNumber, globalTime, actualBatch, actualProcess, finishedProcesses);
        processCont++;

        if (processCont == batches[batchesCont].size()){
            processCont = 0;
            finishedProcesses.push_back("---- End Batch "+to_string(batchesCont+1)+" ----");
            batchesCont++;
            if (batchesCont < totalBatches){
                finishedProcesses.push_back("---- Start Batch "+to_string(batchesCont+1)+" ----");
            }
        }
    }
}

vector<string> printActualBatch(int batchesCont){
    vector<string>actualBatch;
    actualBatch.push_back("Current Batch");
    actualBatch.push_back("Name:   EMT: ");

    for (int i=0;i<batches[batchesCont].size();i++){
        actualBatch.push_back(batches[batchesCont][i].getName()+"  "+
                              to_string(batches[batchesCont][i].getEstimatedTime()));
    }
    return actualBatch;
}

vector<string> printActualProcess(int batchesCont, int processCont){
    vector<string>actualProcess;
    const Process& process = batches[batchesCont][processCont];

    actualProcess.push_back("Actual Process");
    actualProcess.push_back("Name: "+process.getName());
    actualProcess.push_back("Ope: "+to_string(process.getOperand1())+" "+
                            string(1,process.getOperation())+" "+
                            to_string(process.getOperand2()));
    actualProcess.push_back("EMT: "+to_string(process.getEstimatedTime())+" s");
    actualProcess.push_back("ID: "+to_string(process.getId()));
    actualProcess.push_back("ET: "+to_string(process.getElapsedTime())+" s");
    actualProcess.push_back("RT: "+to_string(process.getRemainingTime())+" s");
    return actualProcess;
}

vector<string> printFinishedProcesses(){
    vector<string>finishedProcesses;
    finishedProcesses.push_back("Finished Processes");
    finishedProcesses.push_back("ID    OPE        RES   BatchNumber");
    return finishedProcesses;
}

void renderScreen(int pendingBatchesNumber,int globalTime,const vector<string> &actualBatch,
                  const vector<string> &actualProcess, const vector<string> &finishedProcesses){
   int maxLines = max(max(actualBatch.size(),actualProcess.size()),finishedProcesses.size());
   int const columnASize = 25, columnBSize = 35,columnCSize=30;
   string lineA,lineB,lineC;
   cout << "\n---------------------------------------------------------" << endl;
    cout << "Number of remaining batches: " << pendingBatchesNumber << endl;
    cout << "Global time: " << globalTime << " s" << endl << endl;

    for(int i=0;i<maxLines;i++){
        lineA = (i<actualBatch.size()) ? actualBatch[i] : "";
        lineB = (i<actualProcess.size()) ? actualProcess[i] : "";
        lineC = (i<finishedProcesses.size()) ? finishedProcesses[i] : "";
        cout << setw(columnASize) << left << lineA << setw(columnBSize) << left << lineB
        << setw(columnCSize) << left << lineC << endl;
    }
    cout << "---------------------------------------------------------" << endl;
}

void clearScreen(){
    cout << "\033[2J\033[H";
}
