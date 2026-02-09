# operating-systems-simulations

## Program 1: Batch Processing Simulation

This project is part of the **Operating Systems** course.  
In this first stage, a **batch processing simulation** is implemented, where processes are grouped into batches with a **maximum capacity of 4 processes per batch**.  
Each process is executed in the order it is entered, and the program displays the system state in the console (current batch, current process, finished processes, and global time).

## Console Screen Clearing (ANSI Escape Codes)

This program uses ANSI escape codes to clear the console screen:
cout << "\033[2J\033[H";

This works in most modern terminals (such as Git Bash, Windows Terminal, and Linux/macOS terminals).
In some environments, the screen may not clear properly. In that case, the program will still run correctly, but the output will be displayed without clearing the previous content.
