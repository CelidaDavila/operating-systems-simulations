//Celida Davila
//First Come First Served (FCFS)

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "System.h"

using namespace std;

System os;

void registerProcesses(int numProcesses);
int randInRange(int lowBound, int highBound);
void createProcess(char operation,int operand1,int operand2,int estimatedTime,int id);
int validateSecondOperand(int operand2,char operation);

int main()
{
    srand((unsigned)time(nullptr));
    int numProcesses;
    cout << "How many processes do you want to create?: ";
    cin >> numProcesses;
    cin.get();
    registerProcesses(numProcesses);
    os.run();
    cout << "Press ENTER to list all process info...." << endl;
    cin.get();
    os.printAllInfo();
    cout << "Press ENTER to finish...." << endl;
    cin.get();

    return 0;
}

void registerProcesses(int numProcesses){
    char operations[] = {'+', '-', '*', '/', '%', '^'};
    char operation;
    int cont = 0,estimatedTime,operand1,operand2;
    int id = 0;

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
    os.addNewProcess(process);

}

int validateSecondOperand(int operand2,char operation){
    if ((operation == '/' || operation == '%') && operand2 == 0){
        operand2 = randInRange(-100, 100);
    }
    return operand2;
}



