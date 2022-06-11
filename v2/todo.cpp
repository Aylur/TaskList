#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/types.h> //home dir
#include <pwd.h>
using namespace std;

class Date{
    public:
    int month, day, hour;
    Date(string str){
        if(str.empty()){ this->month = 0; this->day = 0; this->hour = 0; }
        else if(str.size() != 11){ throw 10; } //Date(str) str.size() != 11
        else{
            int m = 0; int d = 0; int h = 0; int min = 0;
            if(str[2] == '/'){
                try{ m = stoi(str.substr(0,2));
                }catch(invalid_argument const& ex){}
                try{ d = stoi(str.substr(3,2));
                }catch(invalid_argument const& ex){}
            }
            if(str[8] == ':'){
                try{ h = stoi(str.substr(6,2))*60;
                }catch(invalid_argument const& ex){}
                try{ min = stoi(str.substr(9,2));
                }catch(invalid_argument const& ex){}
            }
            this->month = m; this->day = d; this->hour = h+min;
            check();
        }
    }
    Date(){
        this->month = 0; this->day = 0; this->hour = 0;
    }
    Date(int month, int day, int hour){
        this->month = month; this->day = day; this->hour = hour;
        check();
    }
    void check(){
        if(month < 0 || month > 12){ throw 1; } //Wrong Month
        if(hour < 0){ throw 2; } //Negative hour
        if(day < 0){ throw 3; } //Negative day
        if(month==1 || month==3 || month==5 || month==7 || month==8 || month==10 || month==12){ if(day>31){ throw 4; } } //day>31
        if(month==4 || month==6 || month==6 || month==9 || month==11){ if(day>30){ throw 5; } } //day>30  
        if(month==2){ if(day>29){ throw 6; } }//february
        if(hour/60 > 24){ throw 7; }//Wrong hour
        if(hour%60 >= 60){ throw 7; }//Wrong hour
    }
    string monthSlashDay(){
        stringstream os;
        if(month == 0){ os << "--"; }
        else if(month < 10){ os << "0" << month;}
        else { os << month; }
        os << "/";

        if(day == 0){ os << "--"; }
        else if(day < 10){ os << "0" << day; }
        else { os << day; }
        
        return os.str();
    }
    string hourColonMin(){
        stringstream os;
        if(hour == 0){ os << "--:--"; }
        else{

            int h = hour/60;
            int min  = hour%60;

            if(h == 0){ os << "00"; }
            else if(h < 10){ os << "0" << h; }
            else { os << h; }
            os << ":";

            if(min == 0){ os << "00"; }
            else if(min < 10){ os << "0" << min; }
            else { os << min; }
        }
        return os.str();
    }
    friend ostream& operator<<(ostream& os, Date& d);
};
ostream& operator<<(ostream& output, Date& d){

    if(d.month == 0 && d.day == 0 && d.hour == 0){
        return output << "           "; //11
    }

    string str = d.monthSlashDay() +" "+ d.hourColonMin();
    size_t pos = str.find("--/--");
    if(pos!=string::npos){ str.replace(pos, 5, "     "); }
    pos = str.find("--:--");
    if(pos!=string::npos){ str.replace(pos, 5, "     "); }

    return output << str;
}

class Task{
    public:
    string task;
    Date date;
    Task(string task, Date date){
        this->task = task;
        this->date = date;
    }
    Task(string str){
        if( str.size() > 12 &&
            (str[2]=='/' || str[2]==' ')&&
            str[5]==' ' &&
            (str[8]==':' || str[8]==' ')&&
            str[11]==' '){
                Date date(str.substr(0,11));
                this->date = date;
                this->task = str.substr(12,str.size());
        }
        else{
            Date date;
            this->date = date;
            this->task = str.substr(0,str.size());
        }
    }
};

string getDir(){
    const char* homedir;
    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }
    string dir(homedir);
    dir += "/Documents/todo.txt";
    return dir;
}

vector<Task> readFile(){
    ifstream input(getDir());
    string task;
    vector<Task> tasks;
    while(getline(input, task)){
        Task ctask(task);
        tasks.push_back(ctask);
    }
    input.close();
    return tasks;
}

void writeFile(vector<Task> tasks){
    ofstream output(getDir());
    for(int i=0; i<tasks.size(); i++){
        output << tasks.at(i).date << " " << tasks.at(i).task << endl;
    }
    output.close();
}

vector<string> readInput(){
    string str;
    getline(cin, str);
    istringstream line(str);
    vector<string> tokens;
    for (string each; getline(line, each, ' '); tokens.push_back(each));
    return tokens;
}

void printCommandList(){
    cout<< "\tq || quit \t to quit       \n";
    cout<< "\th || help \t list commands \n";
    cout<< "\ta || add  \t to add task   \t add [task]\n";
    cout<< "\td || due  \t add due date  \t due [index] [--/--] [--:--]\n";
    cout<< "\tr || rem  \t to remove task\t rem [index]\n";
    cout<< "\tm || mod  \t modify task   \t mod [index] [task]\n";
    cout<< "\ts || sort \t sort tasks    \n";
}

void printTasks(vector<Task> tasks){
    cout << "|Date Hour|   |Tasks|\n\n"; 
    //tasks;
    for(int i=0; i<tasks.size(); i++){
        cout << tasks.at(i).date << " " << i+1 << ". " << tasks.at(i).task << "\n";
    }
    cout << endl;
}

void addTask(vector<string> commands){
    if(commands.size() <= 1){ throw 18; }
    vector<Task> tasks = readFile();

    string task;
    for(int i=1; i<commands.size(); i++){
        if(commands.at(i) == ";"){
            tasks.push_back(task);
            task.clear();
        }else{
            task += commands.at(i) + " ";
        }
    }

    tasks.push_back(task);
    writeFile(tasks);
}

int checkIndex(string command, int size){
    int index;
    try{ index = stoi( command )-1;
    }catch(invalid_argument const& ex){ throw 15; } // invalid arg: [index] not a digit
    if(index < 0 || index > size ){ throw 11; } // invalid [index]
    return index;
}

void remTask(vector<string> commands){
    vector<Task> tasks = readFile();

    for(int i=1; i<commands.size(); i++){
        int index = checkIndex(commands.at(i), tasks.size() );
        tasks.erase(tasks.begin() + index);

    }
    writeFile(tasks);
}

void dueTask(vector<string> commands){
    vector<Task> tasks = readFile();

    if(commands.size() <= 2){ throw 13; } // few  args
    if(commands.size() > 4 ){ throw 14; } // more args
    if(commands.size() == 3 && commands.at(2).size() != 5){ throw 16; } // bad date format
    if(commands.size() == 4 && commands.at(2).size() != 5 && commands.at(3).size() != 5) { throw 16; } // bad date format
    
    int index = checkIndex(commands.at(1), tasks.size() );

    string date = tasks.at(index).date.monthSlashDay();
    string hour = tasks.at(index).date.hourColonMin();
    if(commands.at(2)[2] == '/'){ date = commands.at(2); }
    if(commands.at(2)[2] == ':'){ hour = commands.at(2); }        
    if(commands.size() == 4){
        if(commands.at(3)[2] == '/'){ date = commands.at(3); }
        if(commands.at(3)[2] == ':'){ hour = commands.at(3); }     
    }
    if(date+hour == "          "){ throw 17; } // bad date format

    Date fromString(date+" "+hour);
    tasks.at(index).date = fromString;
    writeFile(tasks);
}

void modTask(vector<string> commands){
    if(commands.size() <= 2){ throw 19; }
    vector<Task> tasks = readFile();
    int index = checkIndex(commands.at(1), tasks.size());

    string newTask;
    for(int i=2; i<commands.size(); i++){
        newTask += commands.at(i)+" ";
    }

    tasks.at(index).task = newTask;
    writeFile(tasks);
}

void sortTasks(){
    vector<Task> tasks = readFile();
    vector<Task> toWrite;

    while(!tasks.empty()){
        Task max = tasks.at(0);
        int maxi = 0;
        int i = 1;
        for(i=1; i<tasks.size(); i++){
            if(max.date.month < tasks.at(i).date.month){max=tasks.at(i);maxi=i;}
            else if(max.date.day < tasks.at(i).date.day){max=tasks.at(i);maxi=i;}
            else if(max.date.hour < tasks.at(i).date.hour){max=tasks.at(i);maxi=i;}
        }
        toWrite.push_back(max);
        tasks.erase(tasks.begin()+maxi);
    }
    writeFile(toWrite);
}

void execute(vector<string> commands){
    string cmd = commands.at(0);
    if(cmd == "q" || cmd == "quit" || cmd == "exit" ){ exit(0); }
    else if(cmd == "h" || cmd == "help"){ throw 100; } //printCommandList();
    else if(cmd == "a" || cmd == "add" ){ addTask(commands); }
    else if(cmd == "r" || cmd == "rem" || cmd == "rm" || cmd == "remove"){ remTask(commands); }
    else if(cmd == "d" || cmd == "due" ){ dueTask(commands); }
    else if(cmd == "m" || cmd == "mod" ){ modTask(commands); }
    else if(cmd == "s" || cmd == "sort"){ sortTasks(); }
    else{ throw 101; } // "Unknown command, type \"h\" or \"help\" for command list\n"
}

string errMessage(int errCode){
    string err;
    if(errCode == 10){ err = "(10)Wrong date format\n"; }

    if(errCode == 1){ err = "Wrong Month\n"; }
    if(errCode == 2){ err = "Negative hour\n"; }
    if(errCode == 3){ err = "Negative day\n"; }
    if(errCode == 4){ err = "day>31\n"; }
    if(errCode == 5){ err = "day>30\n"; }
    if(errCode == 6){ err = "february\n"; }
    if(errCode == 7){ err = "Wrong hour\n"; }

    if(errCode == 11){ err = "Invalid [index]\n"; }
    if(errCode == 12){ err = "(12)[index] needs a digit\n"; }
    if(errCode == 13){ err = "(13)Missing arguments\n"; }
    if(errCode == 14){ err = "Too many arguments\n"; }
    if(errCode == 15){ err = "(15)[index] needs a digit\n"; }
    if(errCode == 16){ err = "(16)Wrong date format\n"; }
    if(errCode == 17){ err = "(17)Wrong date format\n"; }
    if(errCode == 18){ err = "(18)Missing arguments\n"; }
    if(errCode == 19){ err = "(19)Missing arguments\n"; }

    if(errCode == 100){ err = ""; printCommandList(); }
    if(errCode == 101){ err = "Unknown command, type \"h\" or \"help\" for command list\n"; }

    return err;
}

void mainConsole(){
    system("clear");
    printTasks(readFile());
    while(true){
        try{
            cout << "Command:";
            execute(readInput());
            system("clear");
            printTasks(readFile());
        }catch(int err){
            cout << errMessage(err);
        }
    }
}

int main(int argc, char *argv[]){
    vector<string> args(argv, argv+argc);
    if(argc == 1){ mainConsole(); }
    else if(argc == 2){
        if(args.at(1) == "-h"){ printCommandList(); }
        if(args.at(1) == "-l"){ printTasks(readFile()); }
    }
    else{
        args.erase(args.begin());
        try{ execute(args);
        }catch(int err){ cout << errMessage(err); }
    }
    return 0;
}