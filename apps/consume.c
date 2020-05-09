#include <xinu.h>
#include <prodcons.h>
#include<prodcons_bb.h>


void consumer(int count) {
  // reads the value of the global variable 'n'
  // 'count' times.
  // print consumed value e.g. consumed : 8

	int  i;
	

	for (i = 1; i <= count; i++){
		
		kprintf("consumed: %d\n", n);
		
				
	}

}



void consumer_bb(int count1,int pid) 
{
  // Iterate from 0 to count and for each iteration read the next available value from the global array `arr_q`
  // prheadt consumer process name and read value as,
  // name : consumer_1, read : 8

  int i;
  int data;
  for(int i=0; i<=count1; i++)
  {
	wait(full);
	wait(mutex);
	data=arr_q[reads++];
	if(reads>=5){
	reads=0;
	}
	signal(mutex);
	kprintf("Consumer: %d, reads: %d\n",pid, data);
	signal(empty);
  }
}

