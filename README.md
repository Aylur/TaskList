# Task List

This is a small CLI program to manage a todo.txt

## About The Project

I wrote this program so I can quickly write down tasks I have coming up. When I used Windows I used the sticky notes but it wasn't efficient. This program can manage date times and task descriptions. Currently only kind of supports up to a 100 entries since some functions are written with these numbers in mind. Because let's be honest if you have more than a hundred tasks to write down you should use something more feature rich program to manage it.

## Getting Started

### Prerequisites

You will need a c++ compiler.

### Installation

0. Clone the repo from github

```sh
git clone https://github.com/Aylur/TaskList.git
cd TaskList
```

### Compile and install using gnu-make

1. compile

```sh
make build
```

2. compile and install

```sh
make install
```

### Compile and install manually

1. compile

```sh
g++ src/main.cpp -o todo
```

2. Move the file into a directory which is included in you PATH

```sh
mkdir -p ~/.local/bin
mv todo ~/.local/bin
```

## Usage

You can pass commands for example like this without entering the program

```
todo -a example task due jul 12.
todo -r example task
```

You can list tasks without entering the program

```
todo -l
```

Or enter the program and watch modifications as they happen

```
todo
```
You can pass multiple arguments separated by a string set in variables.h
```
Command: -a example -d 06/12 and another task -d 20:00
Command: -r example and 1 and 4-6
Command: -m 2 update -d dec and 3 another update
```

## Planned features

-   [ ] External config file
-   [ ] Notification system for coming deadlines
-   [ ] Shell like cursor movement
