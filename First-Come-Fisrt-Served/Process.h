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
    int arrivalTime;
    int finishTime;
    int returnTime;
    int responseTime;
    int waitingTime;
    int serviceTime;
    int blockedTime;
    int id;
    string status;
    bool error;
    bool responseRegistered;

public:
    Process(char operation,int operand1, int operand2,int estimatedTime,int id){
        this-> operation = operation;
        this-> operand1= operand1;
        this-> operand2 = operand2;
        this-> estimatedTime = estimatedTime;
        this-> remainingTime = estimatedTime;
        this-> elapsedTime = 0;
        this->arrivalTime = 0;
        this->finishTime = -1;
        this->returnTime = 0;
        this->responseTime = 0;
        this->waitingTime = 0;
        this->serviceTime = 0;
        this->blockedTime = 0;
        this->status = "New";
        this->error = false;
        this->id = id;
        this->responseRegistered = false;
    }

    Process(){
    }

    /**Setters**/
    void setStatus(string status){
        this->status = status;
    }
    void setError(bool error){
        this->error = error;
    }
     void setElapsedTime(int elapsedTime){
        this->elapsedTime = elapsedTime;
    }

    void setRemainingTime(int time){
        this->remainingTime=time;
    }
    void setArrivalTime(int arrivalTime){
        this->arrivalTime = arrivalTime;
    }
    void setFinishTime(int finishTime){
        this->finishTime = finishTime;
    }
    void setReturnTime(int returnTime){
        this->returnTime = returnTime;
    }
    void setResponseTime(int responseTime){
        this->responseTime = responseTime;
    }
    void setWaitingTime(int waitingTime){
        this->waitingTime = waitingTime;
    }
    void setServiceTime(int serviceTime){
        this->serviceTime = serviceTime;
    }
    void setBlockedTime(int blockedTime){
        this->blockedTime = blockedTime;
    }
    void setResponseRegistered(bool value){
        this->responseRegistered = value;
    }

    /**Getters**/
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
    int getArrivalTime() const{
        return arrivalTime;
    }
    int getFinishTime() const{
        return finishTime;
    }
    int getReturnTime() const{
        return returnTime;
    }
    int getResponseTime() const{
        return responseTime;
    }
    int getWaitingTime() const{
        return waitingTime;
    }
    int getServiceTime() const{
        return serviceTime;
    }
    int getBlockedTime() const{
        return blockedTime;
    }
    int getId() const{
        return id;
    }
    bool getResponseRegistered(){
        return responseRegistered;
    }
};

#endif // PROCESS_H_INCLUDED
