XINU is an operating system currently being used by some embedded systems. This system is being used as part of our operating systems course here at NCSU.

Basic Overview:
Current XINU as a priority-based scheduler. Priority-based schedulers have the starvation problem where a lower priority process will wait forever if there are new higher priority process coming into the system.

So instead on just getting the process based on priority from the ready queue, as part of this project I implemented two schedulers as mentioned below:
# XINU-Scheduler
1) Random Scheduler
The first scheduling policy is a random schedule. This schedule will choose at random a process eligible for execution. The probability of choosing a specific process will be proportional to its priority. By using random selection we are mitigating the basic problem of priority. This will never cause a starvation problem.

2) Linux-like Scheduler (based loosely on the 2.2 Linux kernel)
This scheduling algorithm tries to loosely emulate the Linux scheduler in 2.2 kernels. In this implementation, we consider all the processes "conventional processes". With this algorithm, the CPU time is divided into epochs. In each epoch, every process has a specified time quantum, whose duration is computed at the beginning of the epoch. An epoch will end when all the runnable processes have used up their quantum. If a process has used up its quantum, it will not be scheduled until the next epoch starts, but a process can be selected many times during the epoch if it has not used up its quantum. 

