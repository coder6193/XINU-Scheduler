# XINU-Scheduler
1) Random Scheduler
The first scheduling policy is a random scheduler. This scheduler will choose at random a process eligible for execution. The probability of choosing a specific process will be proportional to its priority. For example, assume that there are three processes eligible for execution P1, P2, and P3 (each of them is either in the ready list or is the current process) with priorities 30, 20, and 10. The total-sum of those priorities is 60. The goal of the random scheduler is to allocate the CPU with a probability of 30/60 to P1, 20/60 to P2, and 10/60 to P3.

2) Linux-like Scheduler (based loosely on the 2.2 Linux kernel)
This scheduling algorithm tries to loosely emulate the Linux scheduler in 2.2 kernel. In this assignment, we consider all the processes "conventional processes" and uses the policies regarding  SCHED_OTHER specified in the above document. With this algorithm, the CPU time is divided into epochs. In each epoch, every process has a specified time quantum, whose duration is computed at the beginning of the epoch. An epoch will end when all the runnable processes have used up their quantum. If a process has used up its quantum, it will not be scheduled until the next epoch starts, but a process can be selected many times during the epoch if it has not used up its quantum. 

