**Program 1 – Batch Processing Simulation**



This project simulates basic batch processing as part of an Operating Systems course.



The objective is to model how processes are grouped into batches and executed sequentially.



---



\#What Problem Does This Solve?



This program simulates a simplified batch processing system where:


- Processes are grouped into batches with a maximum capacity of 5.

- Batches are executed one at a time.

- Processes inside each batch execute sequentially.

- The system displays its state in real time.



This helps visualize how early operating systems handled batch workloads.



---

\#How It Works



1. The user enters the total number of processes.

2. The system automatically groups them into batches of up to 5 processes.

3. Each process contains:

- Unique ID

- Operation (+, −, \*, /, %, ^)

- Operands

- Estimated Maximum Time (TME)

4. The first batch begins execution.

5. Within the active batch:

- The first process runs until completion.

- Elapsed time (ET) increases every second.

- Remaining time (RT) decreases every second.

6. Once a process finishes:

- Its result is calculated.

- It is moved to the finished processes list.

7. When all processes in a batch complete:

&nbsp;  - The next batch begins automatically.

8. The simulation ends when all batches are fully executed.



Execution is simulated in real time using:

cpp

std::this\_thread::sleep\_for(std::chrono::seconds(1));



---



\#Execution Model


- Processes run in the order they are created.

- No interrupts or multiprogramming are implemented in this version.

- Execution is simulated using real-time delay (`sleep\\\_for`).



---



\#What is displayed on screen



The console shows:



- Current batch (processes waiting)

- Process currently executing

- Finished processes

- Global time counter



Each process includes:

- ID

- Operation

- Estimated Maximum Time (TME)

- Elapsed Time (ET)

- Remaining Time (RT)



---



\#Architecture Decisions



The system is divided into:

- Process: Represents a single job and manages time tracking.

- Batch: Groups processes and manages execution order.

- Simulation: Controls system flow and screen rendering.



This separation keeps the logic organized and easier to extend in later versions.


---



