# Task List
This is a small CLI program to manage a todo.txt

## About The Project

I wrote this program so I can quickly write down tasks I have coming up. When I used Windows I used the sticky notes but it wasn't efficient. This program can manage date times and task descriptions. Currently only kind of supports up to a 100 entries since some functions are written with these numbers in mind. Because let's be honest if you have more than a hundred tasks to write down you should use something more feature rich program to manage it.

## Getting Started

### Prerequisites
You will need a c++ compiler.

### Installation

1. Clone the repo and compile
   ```sh
git clone https://github.com/Aylur/TaskList.git
cd src
g++ main.cpp -o todo
```
2. Move the file into a directory which is included in you PATH
   ```sh
mkdir -p ~/.local/bin
mv todo ~/.local/bin
```

## Usage
You can pass commands for example like this without entering the program
```
todo add example task due jul 12. ; another task -d 9:30
todo rem example task ; another task
```
You can list tasks without entering the program
```
todo -l
todo list
```
Or enter the program and watch modifications as they happen
```
todo
```

## Planned features

- [ ] External config file
- [ ] Notification system for coming deadlines
- [ ] Shell like cursor movement