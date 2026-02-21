//Celida Davila

#include <iostream>
#include <string>
#include <unordered_set>
#include "Simulation.h"

using namespace std;

Simulation simulation;

void registerProcesses(int numProcesses);
void createProcess(string name,char operation,int operand1,int operand2,int estimatedTime,int id);
char validateOperation(char operation);
int validateSecondOperand(int operand2,char operation);
int validateEstimatedTime(int estimatedTime);

int main()
{
    int numProcesses;
    cout << "How many processes do you want to create?: ";
    cin >> numProcesses;
    cin.get();
    registerProcesses(numProcesses);
    simulation.printData();
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
    simulation.addProcess(process);

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

