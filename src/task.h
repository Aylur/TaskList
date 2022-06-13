#pragma once
#include "date.h"

class Task{
    public:
    std::string taskDesc;
    Date date;
    
    Task(std::string taskDesc, Date date){
        this->taskDesc = taskDesc;
        this->date = date;    
    }

    std::string toString(){ return date.toString() + taskDesc;}

    void empty();
    bool isEmpty();

    static Task createFromFileLine(std::vector<std::string> vec);
    static Task createFromInputLine(std::vector<std::string> &input, bool &month, bool &day, bool &minutes);
};

void Task::empty(){
    taskDesc = "";
    Date empty; date = empty;
}

bool Task::isEmpty(){
    if(taskDesc == "") return true;
    else return false;
}

Task Task::createFromFileLine(std::vector<std::string> vec){
    Date date;
    std::string taskDesc;
    int dateCounter = 0;
    while(!vec.empty()){

        if( dateCounter < 2 && date.setMonthDay(vec[0]))
            vec.erase(vec.begin());

        else if( dateCounter < 2 && date.setHourMin(vec[0]))
            vec.erase(vec.begin());

        else{
            taskDesc += vec[0] + " ";
            vec.erase(vec.begin());
        }

        dateCounter++;
    }
    Task task(taskDesc, date);
    return task;
}

Task Task::createFromInputLine(std::vector<std::string> &input, bool &month, bool &day, bool &minutes){
    std::string taskDesc = "";
    Date date;
    month = false; day = false; minutes = false;
    while(!input.empty() && input[0] != LINESEPARATOR){

        if(input[0] == "due" || input[0] == "--due" || input[0] == "-d"){
            input.erase(input.begin());
            int dateCounter = 0;

            while(!input.empty() && dateCounter <4 && input[0] != LINESEPARATOR){
                
                if(date.setHourMin(input[0]))
                    {dateCounter += 2; minutes = true;}
                else if(date.setMonthDay(input[0]))
                    {dateCounter += 2; month = true; day = true;}
                else if(date.setMonth(input[0]))
                    {dateCounter += 1; month = true;} 
                else if(date.setDay(input[0]))
                    {dateCounter += 1; day = true;}

                // else taskDesc += input[0] + " ";
                else{
                    std::string errMsg = "Warning: " + input[0] + " couldn't be parsed as a Date!\n";
                    errorsCaught.push_back({errMsg,0});
                }
                input.erase(input.begin());
            }
        }
        else{
            taskDesc += input[0] + " ";
            input.erase(input.begin());
        }
    }
    Task task(taskDesc, date);
    task.date.check();
    task.date.fix();
    return task;
}