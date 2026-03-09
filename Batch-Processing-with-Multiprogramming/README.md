**Program 2:Batch-Processing-with-Multiprogramming**

---

\#How It Works

Processes are generated internally without input required:

- Unique ID 

- Random Estimated Maximum Time (TME) in range 6–20

- Random operation: + - \* / % ^

- Valid operands depending on operation


Processes are grouped into batches of maximum 5. (batch size can be modified)





\#Execution Model

Each batch is executed sequentially.

Within the active batch:

- One process runs at a time

- Remaining processes stay in the ready queue

- Finished processes are stored separately


When a batch finishes, the next batch begins automatically.



---



\#Keyboard Controls

I - Interrupt (simulates I/O interruption) 

E - Error (Terminates the running process as ERROR)

P - Pause (Pauses the simulation)

C - Continue (Resumes execution after pause)


---


\#What is displayed on screen

The console shows:

- Current batch ready queue (ID, TME, ET)

- Number of pending batches

- Running process (ID, operation, TME, elapsed time, remaining time)

- Finished processes (ID, operation, result or ERROR, batch number, state)

- Global clock



---



\#Architecture Decisions

- Process class: handles individual process data and time tracking.

- Batch class: manages ready queue and execution order.

- Simulation class: controls system flow and user interaction.

- FinishedProcess struct: stores immutable snapshots of completed jobs.



This separation keeps responsibilities clear and improves maintainability.



---



\#Performance Considerations

The simulation runs in real time using `std::this\_thread::sleep\_for(1s)`.


Time complexity is linear relative to the total execution time of all processes.



---

