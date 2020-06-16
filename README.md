# Reader/Writer Problem using InterProcess Communication

## Problem Statement

Write two programs, i.e., Program1.c, and Program2.c that communicate with
each other through a shared memory. Both the programs must be run
indefinitely. For each line of the input (e.g., a sequence of alphanumeric strings)
entered for program 1, program2 should display the *number of vowels,
consonants, words and numerical digits of the input.*

## Instructions
- Compile the file Program1.c and Program2.c using the following commands <br/>
                      1.`gcc -pthread Program1.c -o Program1.exe`<br/>
                      2.`gcc -pthread Program2.c -o Program2.exe`
- Open the Terminal and run the Program1.exe file thus created- `./Program1.exe`
- Open another Terminal and run the Program2.exe file thus created- `./Program2.exe`
- The processes will now start executing. Enter the input in the Terminal for Program1.exe and you will receive the required output in Program2.exe’s terminal window.

## Requirements
- Linux Based Operating System
- Semaphore Suppport (Pthread)
