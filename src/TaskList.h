#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include "task.h"
#include "utils.h"

class TaskList{
    public:
    std::vector<Task> tasks;
    
    TaskList(){}

    void readFromFile();
    void writeToFile();
    void printTasks();

    void sortTasks();

    void addTask(Task task){ tasks.push_back(task); }
    void modTask(int index, Task task, bool month, bool day, bool minutes);
    void remTask(std::vector<int> indexes);

    void parseAdd(std::vector<std::string> input);
    void parseMod(std::vector<std::string> input);
    void parseRem(std::vector<std::string> input);

    int findIndexByDesc(std::string str);

    void parseInput(std::vector<std::string> input);
};

void TaskList::modTask(int index, Task task, bool month, bool day, bool minutes){
    if(!isEmpty(task.taskDesc)) tasks[index].taskDesc = task.taskDesc;
    if(month) tasks[index].date.setMonth(task.date.month);
    if(day) tasks[index].date.setDay(task.date.day);
    if(minutes) tasks[index].date.minutes = task.date.minutes;
}

void TaskList::remTask(std::vector<int> indexes){
    for(int i=0; i<indexes.size(); i++){
        if(indexes[i] > tasks.size() || indexes[i] < 1)
            errorsCaught.push_back({"",32}); //index out of range;
        else
            tasks[indexes[i]-1].empty();
    }
    std::vector<Task> keptTasks;
    for(int i=0; i<tasks.size(); i++){
        if(!tasks[i].isEmpty()) keptTasks.push_back(tasks[i]);
    }
    tasks = keptTasks;
}

int TaskList::findIndexByDesc(std::string toFind){
    int index = 0; int count = 0;
    for(int i=0; i<tasks.size(); i++){
        if(tasks[i].taskDesc.find(toFind) != std::string::npos)
            { index = i+1; count++; }
    }
    if(count == 0) return 0; //didnt find
    if(count != 1) return -1; //multiple matches
    else return index;
}

void TaskList::readFromFile(){
    std::ifstream input(getDir());
    std::string str;
    std::vector<Task> tasks;
    while(getline(input, str)){
        std::vector<std::string> fLine = tokenizer(str);
        tasks.push_back(Task::createFromFileLine(fLine));
    }
    input.close();
    this->tasks = tasks;
}

void TaskList::writeToFile(){
    std::ofstream file(getDir());
    for(int i=0; i<tasks.size(); i++){
        file << tasks[i].toString() << std::endl;
    }
    file.close();
}

void TaskList::printTasks(){
    std::cout << "DeadLine        Tasks\n\n"; 
    for(int i=0; i<tasks.size(); i++){

        std::string sep = "";
        if(i < 9) sep += " " + std::to_string(i+1)+". ";
        else sep += std::to_string(i+1)+ ". ";

        std::cout << tasks[i].date.toString() << sep << tasks[i].taskDesc << std::endl;
    }
    std::cout << std::endl;
}

void TaskList::sortTasks(){
    std::vector<Task> sortedTasks;

    while(!tasks.empty()){
        Task max = tasks[0];
        int maxi = 0;
        int i = 1;
        for(i=1; i<tasks.size(); i++){
            if(max.date.month < tasks.at(i).date.month){max=tasks.at(i);maxi=i;}
            else if(max.date.month == tasks.at(i).date.month){
                if(max.date.day < tasks.at(i).date.day){max=tasks.at(i);maxi=i;}
                else if(max.date.day == tasks.at(i).date.day){
                    if(max.date.minutes < tasks.at(i).date.minutes){max=tasks.at(i);maxi=i;}
                }
            }
        }
        sortedTasks.push_back(max);
        tasks.erase(tasks.begin()+maxi);
    }

    tasks = sortedTasks;
}

void TaskList::parseAdd(std::vector<std::string> input){
    if(input.size() == 0) errorsCaught.push_back({"",30}); //missing arguments

    while(!input.empty()){
        if(input[0] == LINESEPARATOR) input.erase(input.begin());


        bool month, day, minutes;
        Task task = Task::createFromInputLine(input, month, day, minutes);
        addTask(task);
    }
}

void TaskList::parseMod(std::vector<std::string> input){
    if(input.size() == 0) errorsCaught.push_back({"",30}); //missing arguments

    while(!input.empty()){
        if(input[0] == LINESEPARATOR) input.erase(input.begin());

        int toModIndex = 0; bool validIndex = false; bool indexExists = false;
        try{
            toModIndex = std::stoi(input[0]);
            input.erase(input.begin());
            indexExists = true;
            validIndex = toModIndex >= 1 && toModIndex<=tasks.size();
        }catch(const std::exception& e){
           errorsCaught.push_back({"",31}); //missing index arg
        }
        
        if(validIndex){
            bool month, day, minutes;
            Task task = Task::createFromInputLine(input, month, day, minutes);
            modTask(toModIndex-1, task, month, day, minutes);
        }
        if(!validIndex && indexExists)
            errorsCaught.push_back({"",32}); //index out of range

        //clear input untill next separator
        if(!validIndex)
            while(!input.empty() && input[0] != LINESEPARATOR)
                input.erase(input.begin());

    }
}

void TaskList::parseRem(std::vector<std::string> input){
    if(input.size() == 0) errorsCaught.push_back({"",30}); //missing arguments

    std::vector<int> indexesToDelete;

    while(!input.empty()){
        if(input[0] == LINESEPARATOR) input.erase(input.begin());

        //last item if size==1 || input[1]==LINESEP
        bool lastItem = input.size() == 1;
        if(!lastItem) lastItem = input[1] == LINESEPARATOR;

        int index = 0;
        if(lastItem){
            if(getArray(input[0], indexesToDelete)){
                input.erase(input.begin());
            }
            else if(toInt(input[0], index)){
                indexesToDelete.push_back(index);
                input.erase(input.begin());
            }
            else{
                lastItem = false;
            }
        }
        if(!lastItem){
            std::string toFind = "";
            while(!input.empty() && input[0] != LINESEPARATOR){
                toFind += input[0];
                input.erase(input.begin());
                if(!input.empty())
                    { if(input[0] != LINESEPARATOR ) toFind += " "; }
            }
            index = findIndexByDesc(toFind);
            if(index == 0) errorsCaught.push_back({"",34}); //couldnt find this task;
            if(index == -1)errorsCaught.push_back({"Warning: There are multiple matching tasks for \""+toFind+"\"!\n         Please be more specific!\n",0});
            if(index > 0) indexesToDelete.push_back(index);
        }
    }

    remTask(indexesToDelete);
}

void TaskList::parseInput(std::vector<std::string> input){
    std::string cmd;
    if(!input.empty()){
        cmd = input.at(0);
        input.erase(input.begin());
    }
    else cmd = "";   

    if(cmd=="q" || cmd=="quit"  || cmd=="exit" ||
       cmd=="-q"|| cmd=="--quit"|| cmd=="--exit") exit(0); 

    else if(cmd== "h"  || cmd==  "help" ||
            cmd=="-h"  || cmd=="--help"  ) errorsCaught.push_back({"",100}); //printCommandList();

    else if(cmd== "a"  || cmd==  "add"  ||
            cmd=="-a"  || cmd=="--add"   ) parseAdd(input);

    else if(cmd== "r"  || cmd==  "rem"  ||
            cmd=="-r"  || cmd=="--rem"  ||
            cmd==  "rm"|| cmd== "remove"||
            cmd=="--rm"|| cmd=="--remove") parseRem(input);

    else if(cmd== "m"  || cmd==  "mod"  ||
            cmd=="-m"  || cmd=="--mod"   ) parseMod(input);

    else if(cmd== "s"  || cmd==  "sort" ||
            cmd=="-s"  || cmd=="--sort"  ) sortTasks();

    else if(cmd != "") errorsCaught.push_back({"",101}); // "Unknown command, type \"h\" or \"help\" for command list\n"
}