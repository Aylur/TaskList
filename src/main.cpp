#include "TaskList.h"

TaskList list;

void mainConsole(){
    while(true){
        clearConsole();
        list.printTasks();
        list.writeToFile();
        printErrorMessages();
        std::cout << "Command: ";
        list.parseInput(readInput());
    }
}

int main(int argc, char *argv[]){
    std::vector<std::string> args(argv, argv+argc);
    list.readFromFile();

    if(argc == 1){ mainConsole(); }
    else if(argc == 2){
        if(args.at(1) == "-h" || args.at(1) == "--help" ||
           args.at(1) ==  "h" || args.at(1) ==   "help"  ) printCommandList();
        
        if(args.at(1) == "-l" || args.at(1) == "--list" ||
           args.at(1) ==  "l" || args.at(1) ==   "list"  ) list.printTasks(); 
    }
    else{
        args.erase(args.begin());

        try{ list.parseInput(args); }
            catch(int err)
                { std::cout << errMessage(err); }
        
        list.writeToFile();
    }
    return 0;
}