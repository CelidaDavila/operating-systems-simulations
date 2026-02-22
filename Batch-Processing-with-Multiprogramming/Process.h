#ifndef PROCESS_H_INCLUDED
#define PROCESS_H_INCLUDED

#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

class Process{
private:
    char operation;
    int operand1,operand2;
    int estimatedTime;
    int remainingTime;
    int elapsedTime;
    int id;
    string status;
    bool error;

public:
    Process(char operation,int operand1, int operand2,int estimatedTime,int id){
        this-> operation = operation;
        this-> operand1= operand1;
        this-> operand2 = operand2;
        this-> estimatedTime = estimatedTime;
        this-> remainingTime = estimatedTime;
        this-> elapsedTime = 0;
        this->id = id;
        this->status = "Finished";
        this->error = false;
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
    void setStatus(string status){
        this->status = status;
    }
    void setError(){
        if(getStatus() == "Error"){
            error = true;
        }
    }
    void setRemainingTime(int time){
        this->remainingTime=time;
    }
    string getStatus(){
        return status;
    }
    bool getError(){
        return error;
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

#endif // PROCESS_H_INCLUDED
