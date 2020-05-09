#include <xinu.h>
#include <prodcons.h>
#include<prodcons_bb.h>

void producer(int count) {
    // Iterates from 0 to count, setting
    // the value of the global variable 'n'
    // each time.
    //print produced value e.g. produced : 8


	int  i;
	for (i = 1 ; i <= count; i++){
		
		n++;
		kprintf("produced: %d\n", n);
		
	

	}
}



void producer_bb(int count,int pid) 
{
  // Iterate from 0 to count and for each iteration add iteration value to the global array `arr_q`, 
  // print producer process name and written value as,
  // name : producer_1, write : 8

  int i, data=0;
	for(i=0; i<=count; i++)
	{ 	
		wait(empty);
		data++;
		wait(mutex);
		arr_q[writes++]=data;
		if(writes>=5){
		 writes=0;
		}
		kprintf("Producer: %d, writes: %d\n",pid, data);
		signal(mutex);
		signal(full);
	}
}
