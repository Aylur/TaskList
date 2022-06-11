#pragma once
#include "variables.h"
#include "utils.h"

class Date{
    public:
    int month, day, minutes;
    Date(): month(0), day(0), minutes(0) {}

    void setMonth(int month){this->month = month;}
    void setDay(int day){this->day = day;}
    void setHour(int minutes);
    void setMin(int minutes);

    bool setMonth(std::string str);
    bool setDay(std::string str);

    bool setMonthDay(std::string str);
    bool setHourMin(std::string str);

    std::string getMonthDay();
    std::string getHourMin();

    std::string toString();

    void check();
    void fix(); //sets uncorrect month/day/hour to 0
};
void Date::setHour(int minutes){
    int thisMin = this->minutes%60;
    this->minutes = minutes*60 + thisMin;
}
void Date::setMin(int min){
    int thisMin = this->minutes%60;
    int hour = this->minutes - thisMin;
    this->minutes = hour + min;
}

bool Date::setMonth(std::string str){
    int value = getMonthValue(str);
    if(value > 0){
        setMonth(value);
        return true;
    }
    else return false;
}

bool Date::setDay(std::string str){
    int value = getDayValue(str);
    if(value > 0){
        setDay(value);
        return true;
    }
    else return false;
}

bool Date::setMonthDay(std::string str){
    if(str.size() == 5){
        if(str[2] == '/'){
            int m = 0; int d = 0;
            //needs fix -1 through -9 gets parsed 
            try{ m = stoi(str.substr(0,2)); }catch(std::invalid_argument const& ex){}
            try{ d = stoi(str.substr(3,2)); }catch(std::invalid_argument const& ex){}
            setMonth(m); setDay(d);
            return true;
        }
    }
    return false;
}

bool Date::setHourMin(std::string str){
    if(str.size() == 5){
        if(str[2] == ':'){
            int h = 0; int min = 0;
            //needs fix -1 through -9 gets parsed 
            try{ h = stoi(str.substr(0,2)); }catch(std::invalid_argument const& ex){}
            try{ min = stoi(str.substr(3,2)); }catch(std::invalid_argument const& ex){}
            setHour(h); setMin(min);
            return true;
        }
    }
    return false;
}

std::string Date::getMonthDay(){
    if(month == 0 && day == 0) return "";
    std::string str = "";
    if(month == 0){ str += "--"; }
    else if(month < 10){ str += "0" + std::to_string(month);}
    else { str += std::to_string(month); }
    str += "/";

    if(day == 0){ str += "--"; }
    else if(day < 10){ str += "0" + std::to_string(day); }
    else { str += std::to_string(day); }
    
    return str;
}

std::string Date::getHourMin(){
    if(minutes == 0) return "";
    std::string str = "";
    int h = minutes/60;
    int min  = minutes%60;

    if(h == 0){ str += "00"; }
    else if(h < 10){ str += "0" + std::to_string(h); }
    else { str += std::to_string(h); }
    str += ":";

    if(min == 0){ str += "00"; }
    else if(min < 10){ str += "0" + std::to_string(min); }
    else { str += std::to_string(min); }
    return str;
}

std::string Date::toString(){
    if( (month!=0 || day!=0) && minutes!=0)
        return this->getMonthDay() + " " + this->getHourMin()+" ";

    if( (month!=0 || day!=0) && minutes==0)
        return this->getMonthDay() + "       ";
    
    if(month==0 && day==0 && minutes!=0)
        return "      " + this->getHourMin() + " ";

    else return "            ";
}

void Date::fix(){
    if(month < 0 || month > 12) month = 0;
    if(day < 0 || day > 31) day = 0;
    if(minutes/60 > 24 || minutes%60 >= 60) minutes = 0;
    if(minutes > 1439 || minutes < 0) minutes = 0;
    if(day > 31){
        switch (month){
        case 1:  month = 0; break;
        case 3:  month = 0; break;
        case 5:  month = 0; break;
        case 7:  month = 0; break;
        case 8:  month = 0; break;
        case 10: month = 0; break;
        case 12: month = 0; break;
        }
    }
    if(day > 30){
        switch (month){
        case 4:  month = 0; break;
        case 6:  month = 0; break;
        case 9:  month = 0; break;
        case 11: month = 0; break;
        }
    }
    if(day >29 && month==2) month = 0;
}

void Date::check(){
    if(month < 0 || month > 12) throw 20;
    if(minutes/60 > 24 || minutes%60 >= 60) throw 21;
    if(day > 31){
        switch (month){
        case 1:  throw 1;  break;
        case 3:  throw 3;  break;
        case 5:  throw 5;  break;
        case 7:  throw 7;  break;
        case 8:  throw 8;  break;
        case 10: throw 10; break;
        case 12: throw 12; break;
        }
    }
    if(day > 30){
        switch (month){
        case 4:  throw 4;  break;
        case 6:  throw 6;  break;
        case 9:  throw 9;  break;
        case 11: throw 11; break;
        }
    }
    if(day >29 && month==2) throw 2;
}