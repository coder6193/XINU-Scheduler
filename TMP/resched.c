/* resched.c  -  resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
int sched = 0;
unsigned long currSP;	/* REAL sp of current process */
extern int ctxsw(int, int, int, int);
int epoch=0;
/*-----------------------------------------------------------------------
 * resched  --  reschedule processor to highest priority ready process
 *
 * Notes:	Upon entry, currpid gives current process id.
 *		Proctab[currpid].pstate gives correct NEXT state for
 *			current process if other than PRREADY.
 *------------------------------------------------------------------------
 */
int resched()
{
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */

	optr= &proctab[currpid];
	if (optr->pstate == PRCURR) {
		optr->pstate = PRREADY;
		insert(currpid,rdyhead,optr->pprio);
	}
	/* no switch needed if current process priority higher than next*/

	/* force context switch */

	
	//random scheduler starts here
	if(getschedclass()==1){
		int prior_sum=0;
		int next = q[rdyhead].qnext;
		while(q[next].qkey < MAXINT)
		{
			prior_sum=prior_sum+q[next].qkey;
			next=q[next].qnext;
		}

		int prev = q[rdytail].qprev;
		if(prior_sum>0)
		{
			int random_number = rand()%prior_sum;
			while(1)
			{
				if(random_number<q[prev].qkey)
				{
					nptr = &proctab[(currpid = dequeue(prev))];
					break;
				}
				else
				{
					random_number=random_number-q[prev].qkey;
				}
				prev=q[prev].qprev;
			}
		} 
		else{
			nptr = &proctab[(currpid = dequeue(prev))];
		}
		preempt = QUANTUM;
	}
	else if(getschedclass()==2)
	{
		//kprintf("pid: %d\n",currpid);
		int totalgoodness = 0;
		//kprintf("%d ",epoch);
		if(epoch > 0){
			//present process goodness and quantum values had been resetted
			epoch = epoch - optr->quantam + preempt;
			optr->goodness = optr->goodness - optr->quantam + preempt;
			optr->quantam = preempt;
			if(preempt==0)
			{
				optr->goodness = 0;
			}

			//decide whether a new epoch need to be started
			int next = q[rdyhead].qnext;
			//caluclate goodness value here
			while(q[next].qkey < MAXINT && proctab[next].goodness>=0)
			{
				totalgoodness = totalgoodness + proctab[next].goodness;
				next = q[next].qnext;
			}
		}
		//begin a new epoch here
		if(epoch==0 || totalgoodness==0)
		{
			epoch=0;
			//loop to set goodness and quantum to all process
			//kprintf("entered into new epoch\n");
			int i=0;
			int temp=0;
			for(;i<NPROC;i++){
				if(proctab[i].pstate != PRFREE)
				{
					temp = proctab[i].quantam;
					proctab[i].quantam = temp/2 + proctab[i].pprio;
					epoch= epoch + proctab[i].quantam;
					proctab[i].goodness = proctab[i].quantam + proctab[i].pprio;
				}
			}
		}	
		//printf("%d ",epoch);
		//either some new epoch is created or old epoch is being used.
		int highGoodPid = 0;
		int highGood=0;
		int next = q[rdyhead].qnext;

		//caluclate goodness value here
		while(q[next].qkey < MAXINT)
		{
			if(proctab[next].goodness >= highGood){
				highGoodPid = next;
				highGood = proctab[next].goodness;
			}
			//kprintf("%d %d-",proctab[next].goodness,next);
			next = q[next].qnext;
		}
		//printf("s %d %d\n",proctab[highGoodPid].goodness,highGoodPid);
		nptr = &proctab[(currpid = dequeue(highGoodPid))];
		preempt = proctab[highGoodPid].quantam; 

	}
		else{
		if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
	   	(lastkey(rdytail)<optr->pprio)) {
			return(OK);
		}
		nptr = &proctab[ (currpid = getlast(rdytail)) ];
		preempt = QUANTUM;
	}
	/* remove highest priority process at end of ready list */

		nptr->pstate = PRCURR;		/* mark it currently running	*/
#ifdef	RTCLOCK
	//preempt = QUANTUM;		/* reset preemption counter	*/
#endif
	
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	
	/* The OLD process returns here when resumed. */
	return OK;
}

void setschedclass(int sched_class){
	sched=sched_class;
}

int getschedclass(){
	return sched;
}
