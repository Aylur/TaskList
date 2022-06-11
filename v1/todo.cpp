#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <sys/ioctl.h> //consolewidth
#include <unistd.h>
#include <sys/types.h> //home dir
#include <pwd.h>
using namespace std;


vector<string> readFile();
void printTasks(vector<string> tasks);
vector<string> readCommand();
void execute(vector<string> command);



int main(int argc, char *argv[])
{
    printTasks(readFile());
    vector<string> command;
    while(true){
        cout << "Command: ";
        command = readCommand();
        execute(command);
        printTasks(readFile());
    }

    return 0;
}

string getDir(){
    const char* homedir;
    if ((homedir = getenv("HOME")) == NULL) {
    homedir = getpwuid(getuid())->pw_dir;
    }
    string dir(homedir);
    dir += "/Documents/todo.txt";
    return dir;
}

vector<string> readFile(){
    string dir = getDir();
    //read todo.txt
    ifstream input(dir);
    string tmpLine = "";
    vector<string> lines;
    while(getline(input, tmpLine)){
        lines.push_back(tmpLine);
    }
    input.close();
    return lines;
}

void printTasks(vector<string> tasks){
    //window size
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    //#############
    for(int i=0; i<w.ws_col; i++){
        cout << "#";
    }
    cout << endl;

    //tasks;
    for(int i=0; i<tasks.size(); i++){
        cout << "\t" << i+1 << ". " << tasks.at(i) << endl;
    }

    //#############
    for(int i=0; i<w.ws_col; i++){
        cout << "#";
    }
    cout << endl;
}

void printCommandList(){
    cout<< "\tq || quit \t to quit       \n";
    cout<< "\th || help \t list commands \n";
    cout<< "\ta || add  \t to add task   \t add [task]\n";
    cout<< "\tr || rem  \t to remove task\t rem [index]\n";
    cout<< "\tm || mod  \t modify task   \t mod [index] [task]\n";
}

vector<string> readCommand(){
    string str;
    getline(cin, str);
    istringstream line(str);
    vector<string> tokens;
    for (string each; getline(line, each, ' '); tokens.push_back(each));
    return tokens;
}

void writeFile(vector<string> tasks){
    string dir = getDir();
    ofstream output(dir);
    for(int i=0; i<tasks.size(); i++){
        output << tasks.at(i) << endl;
    }
    output.close();
}

void addTask(vector<string> command){
    vector<string> tasks = readFile();
    string str;
    for(int i=1; i<command.size(); i++){
        if(command.at(i) == ";"){
            tasks.push_back(str);
            str.clear();
        }else{
            str += command.at(i) + " ";
        }
    }
    tasks.push_back(str);
    writeFile(tasks);
}

void remTask(vector<string> command){
    vector<string> tasks = readFile();
    for(int i=1; i<command.size(); i++){
        tasks.erase(tasks.begin() + stoi( command.at(i) )-1 );
    }
    writeFile(tasks);
}

void execute(vector<string> command){
    if(command.at(0) == "q" || command.at(0) == "quit" || command.at(0) == "exit" ){ exit(0); }
    else if(command.at(0) == "h" || command.at(0) == "help"){ printCommandList(); }
    else if(command.at(0) == "a" || command.at(0) == "add" ){ addTask(command); }
    else if(command.at(0) == "r" || command.at(0) == "rem" || command.at(0) == "remove"){ remTask(command); }
    else{
        cout << "Unknown command, type \"h\" or \"help\" for command list\n";
    }
}