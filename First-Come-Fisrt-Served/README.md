**Program 3:First Come Fisrt Served**

---

\#How It Works

Processes are generated internally without input required:

- Unique ID 

- Random Estimated Maximum Time (TME) in range 6–20

- Random operation: + - \* / % ^

- Valid operands depending on operation

(Batches no longer exists)

Each generated process is added to the system in the New state, waiting to enter memory.


---

\#Execution Model

The system follows the Five-State Process Model:

New – Process has been created but not yet loaded into memory.

Ready – Process is waiting for CPU execution.

Running – Process currently using the CPU.

Blocked – Process temporarily suspended due to simulated interruption.

Finished – Process completed successfully or terminated with an error.

NOTE: Only 5 processes can be in memory simultaneously (Ready + Running + Blocked).


---



\#Keyboard Controls

I – Interrupt the running process (moves it to the Blocked state)

E – Force an error in the running process

P – Pause the simulation

C – Continue the simulation

N - Add new process

B - Show PCB (Process Control Block)

---


\#What is displayed on screen

The console shows:

Global Information

Global system clock

Number of processes still in the New state

Ready Queue

Displays processes waiting for CPU execution:

Process ID

Estimated Maximum Time (EMT)

Elapsed Time (ET)

Running Process

Information about the currently executing process:

ID

Estimated execution time

Elapsed time

Remaining time

Finished Processes

Processes that completed execution:

Process ID

Operation

Result (or ERROR)

Final state

Blocked Processes

Processes temporarily interrupted, including their blocked time.



---



\#Architecture Decisions

The program uses a modular design to separate responsibilities:

Process class :Represents an individual process and stores it's data
System class: Handles the operating system simulation such as different process data structures, scheduling logic, CPU dispatching, state transitions and keyboard event handling.

Queues and containers:

queue<Process> for New and Ready states

vector<Process> for Finished and Blocked processes

This separation improves readability, scalability, and maintainability.


---



\#Performance Considerations

The simulation runs in real time using `std::this\_thread::sleep\_for(1s)`

The algorithm complexity is linear relative to the total execution time of all processes, since each process is updated once per tick.


---

