#pragma once
#include <pwd.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <iostream>

std::string getDir(){
    const char* homedir;
    if ((homedir = getenv("HOME")) == NULL) //linux /home/user
        homedir = getenv("USERPROFILE");    //win   C:\Users\user

    std::string dir(homedir);
    dir += FILELOCATION;
    return dir;
}

bool isEmpty(std::string str){
    for(int i=0; i<str.size(); i++){
        if(str.at(i) != ' ') return false;
    }
    return true;
}

bool equalStr(std::string str, std::string str2){
    if(str.size() != str2.size()) return false;

    for(int i=0; i<str.size(); i++){
        if(str[i] != std::tolower(str2[i]) && str[i] != std::toupper(str2[i]))  
            return false;
    }

    return true;
}

bool containsStr(std::string inStr, std::string str){
    if (inStr.find(str) != std::string::npos)
        return true;

    return false;
}

bool toInt(std::string str, int &index){
    bool b = true;
    try{ index = std::stoi(str); }
        catch(const std::exception& e)
            { b = false; index = 0; }
    return b;
}

bool getArray(std::string str, std::vector<int> &indexesToDelete){
    int fromStart = 0; int fromLength = 0;
    int toStart = 0; int toLength = 0;

    switch(str.size()){
    case 3: //eg 1-4 
        fromStart = 0; fromLength = 1;
        toStart = 2; toLength = 1;
        break;
    case 4: //eg 9-12
        fromStart = 0; fromLength = 1;
        toStart = 2; toLength = 2;
        break;
    case 5: //eg 19-24
        fromStart = 0; fromLength = 2;
        toStart = 3; toLength = 2;
        break;
    default:
        return false;
    }
    if(str.size() == 3 || str.size() == 4 || str.size() == 5 ){
        int from = 0; int to = 0;
        bool bFrom = true; bool bTo = true;
        try{ from = std::stoi(str.substr(fromStart,fromLength)); }
            catch(const std::exception& e){ bFrom = false; }
        try{ to = std::stoi(str.substr(toStart, toLength)); }
            catch(const std::exception& e){ bTo = false; }
        
        if( bFrom && bTo && from <= to){
            for(from; from <= to; from++)
                indexesToDelete.push_back(from);
            return true;
        }
        else return false;
    }
    return false;
}

std::vector<std::string> tokenizer(std::string s)
{
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string word;
    while (ss >> word) {
        tokens.push_back(word);
    }
    return tokens;
}

std::vector<std::string> readInput(){
    std::string str;
    std::getline(std::cin, str);
    lastInputLine = "Command: " + str + "\n";
    return tokenizer(str);
}

void printCommandList(){
    std::cout<< "Command list:\n";
    std::cout<< "\tq || quit \t to quit       \n";
    std::cout<< "\th || help \t list commands \n";
    std::cout<< "\ta || add  \t to add task   \t add [task] due [date]\n";
    std::cout<< "\t\t[date] format: --/-- --:--\n";
    std::cout<< "\tr || rem  \t to remove task\t rem [index] || rem [task description]\n";
    std::cout<< "\tm || mod  \t modify task   \t mod [index] [task] due [date]\n";
    std::cout<< "\ts || sort \t sort tasks    \n";
}

std::string errMessage(int errCode){
    std::string err = "Warning: ";

    switch(errCode){
        case  1: err += "January only has months 31 in it\n"; break;
        case  2: err += "February only has months 29 in it\n"; break;
        case  3: err += "March only has months 31 in it\n"; break;
        case  4: err += "April only has months 30 in it\n"; break;
        case  5: err += "May only has months 31 in it\n"; break;
        case  6: err += "June only has months 30 in it\n"; break;
        case  7: err += "July only has months 31 in it\n"; break;
        case  8: err += "August only has months 31 in it\n"; break;
        case  9: err += "September only has months 30 in it\n"; break;
        case 10: err += "Oktober only has months 31 in it\n"; break;
        case 11: err += "November only has months 30 in it\n"; break;
        case 12: err += "December only has months 31 in it\n"; break;

        case 20: err += "There are only 12 months in a year\n"; break; // -1 through -9 gets parsed and this is thrown
        case 21: err += "There are only 24 hours in a day\n"; break;



        case 30: err += "Missing arguments\n"; break;
        case 31: err += "Missing specified index\n"; break;
        case 32: err += "Index out of range\n"; break;
        case 33: err += "Could not find this index\n"; break;
        case 34: err += "Could not find a task with this description\n"; break;
        case 35: err += "There are multiple matching tasks, please be more specific\n"; break;
        case 100: err = ""; printCommandList(); break;
        case 101: err += "Unknown command, type \"h\" or \"help\" for command list\n"; break;
    }
    return err;
}

void printErrorMessages(){
    if(!errorsCaught.empty())
        std::cout << lastInputLine;
    for(int i=0; i<errorsCaught.size(); i++){
        std::cout << errMessage(errorsCaught[i]);
    }
    errorsCaught.clear();
}

//variables
int getMonthValue(std::string str){
    for(int i=0; i<monthNameList.size(); i++){
        if(equalStr(monthNameList[i].name, str))
            return monthNameList[i].value;
    }
    return 0;
}

int getDayValue(std::string str){
    for(int i=0; i<dayNameList.size(); i++){
        if(equalStr(dayNameList[i].name, str))
            return dayNameList[i].value;
    }
    return 0;
}

// unused
// std::string getDayName(int value){
//     for(int i=0; i<dayNameList.size(); i++){
//         if(dayNameList[i].value == value)
//             return dayNameList[i].name;
//     }
//     return "MISSING DAYNAME";
// }
// std::string getMonthName(int value){
//     for(int i=0; i<monthNameList.size(); i++){
//         if(monthNameList[i].value == value)
//             return monthNameList[i].name;
//     }
//     return "MONTH MISSING";
// }