#include <xinu.h>
#include <prodcons_bb.h>
#include <stdio.h>  
#include <stddef.h>
#include <stdlib.h>
#include<future.h>
#include<future_test.h>
#include<stream.h>
#include<tree.h>



//function declaration
void prodcons_bb(int nargs , char *args[]);
void futures_test(int nargs,char *args[]);
int stream_proc(int nargs, char *args[]);
int stream_proc_futures(int nargs, char* args[]);
uint fstest(int nargs, char *args[]);
int ffib(int n);

int one=1;
int zero=0;
int two=2;
 int three = 3, four = 4, five = 5, six = 6;
  int seven = 7, eight = 8, nine=9, ten = 10, eleven = 11;
  int id =1;
future_t **fibfut;


int32 arr_q[5];
sid32 full, mutex, empty;
int32 reads, writes;                 
int32 data;


void future_fib(int nargs, char*args[]){

	int fib = -1, i;

    fib = atoi(args[2]);

    if (fib > -1) {
      int final_fib;
      int future_flags = FUTURE_SHARED; 

      // create the array of future pointers
      if ((fibfut = (future_t **)getmem(sizeof(future_t *) * (fib + 1)))
          == (future_t **) SYSERR) {
        printf("getmem failed\n");
        return(SYSERR);
      }

      // get futures for the future array
      for (i=0; i <= fib; i++) {
        if((fibfut[i] = future_alloc(future_flags, sizeof(int), 1)) == (future_t *) SYSERR) {
          printf("future_alloc failed\n");
          return(SYSERR);
        }
      }

      // spawn fib threads and get final value
      // TODO - you need to add your code here
		for(i=0; i <= fib; i++){
			resume( create(ffib, 1024, 20, "ffib", 1, i) );
		}

      future_get(fibfut[fib], (char*) &final_fib);

      for (i=0; i <= fib; i++) {
        future_free(fibfut[i]);
      }

      freemem((char *)fibfut, sizeof(future_t *) * (fib + 1));
      printf("Nth Fibonacci value for N=%d is %d\n", fib, final_fib);
      return(OK);
	}
	}
	

void futures_test(int nargs, char *args[])
{	
	future_t *f_exclusive, *f_shared;
          
            f_exclusive = future_alloc(FUTURE_EXCLUSIVE,sizeof(int), 1);
            f_shared = future_alloc(FUTURE_SHARED,sizeof(int), 1);
           
            
            // Test FUTURE_EXCLUSIVE
           resume( create(future_cons, 1024, 20, "fcons1", 1, f_exclusive) );
           resume( create(future_prod, 1024, 20, "fprod1", 2, f_exclusive,&one) );
	   
            
            // Test FUTURE_SHARED
             resume( create(future_cons, 1024, 20, "fcons2", 1, f_shared) );
             resume( create(future_cons, 1024, 20, "fcons3", 1, f_shared) );
             resume( create(future_cons, 1024, 20, "fcons4", 1, f_shared) );
             resume( create(future_cons, 1024, 20, "fcons5", 1, f_shared) );
             resume( create(future_prod, 1024, 20, "fprod2", 2, f_shared, &two) );
	
}


void tscdf(int nargs, char *args[]){

 
 resume(create(stream_proc,4096,20,"stream_proc",2,nargs,args));
 
}

void tscdf_fq(int nargs, char *args[]){

 
 resume(create(stream_proc_futures,4096,20,"stream_proc_futures",2,nargs,args));
 
}

void futureq_test1 (int nargs, char *args[]) {
   // int three = 3, four = 4, five = 5, six = 6;
   
    future_t *f_queue;
    f_queue = future_alloc(FUTURE_QUEUE, sizeof(int), 3);

    resume(create(future_cons, 1024, 20, "fcons6", 1, f_queue));
    resume(create(future_cons, 1024, 20, "fcons7", 1, f_queue));
    resume(create(future_cons, 1024, 20, "fcons8", 1, f_queue));
    resume(create(future_cons, 1024, 20, "fcons9", 1, f_queue));
    resume(create(future_prod, 1024, 20, "fprod3", 2, f_queue, (char *)&three));
    resume(create(future_prod, 1024, 20, "fprod4", 2, f_queue, (char *)&four));
    resume(create(future_prod, 1024, 20, "fprod5", 2, f_queue, (char *)&five));
    resume(create(future_prod, 1024, 20, "fprod6", 2, f_queue, (char *)&six));
  //  resume(create(future_cons, 1024, 20, "fprod7", 1, f_queue));
}

void futureq_test2 (int nargs, char *args[]) {
   // int seven = 7, eight = 8, nine=9, ten = 10, eleven = 11;
    
    future_t *f_queue;
    f_queue = future_alloc(FUTURE_QUEUE, sizeof(int), 3);

    resume(create(future_prod, 1024, 20, "fprod10", 2, f_queue, (char *)&seven));
    resume(create(future_prod, 1024, 20, "fprod11", 2, f_queue, (char *)&eight));
    resume(create(future_prod, 1024, 20, "fprod12", 2, f_queue, (char *)&nine));
    resume(create(future_prod, 1024, 20, "fprod13", 2, f_queue, (char *)&ten));
    resume(create(future_prod, 1024, 20, "fprod13", 2, f_queue, (char *)&eleven));

    resume(create(future_cons, 1024, 20, "fcons14", 1, f_queue));
    resume(create(future_cons, 1024, 20, "fcons15", 1, f_queue));
    resume(create(future_cons, 1024, 20, "fcons16", 1, f_queue));
    resume(create(future_cons, 1024, 20, "fcons17", 1, f_queue));
    resume(create(future_cons, 1024, 20, "fcons18", 1, f_queue));
}

void futureq_test3 (int nargs, char *args[]) {
    int three = 3, four = 4, five = 5, six = 6;
    
    future_t *f_queue;
    f_queue = future_alloc(FUTURE_QUEUE, sizeof(int), 3);

    resume( create(future_cons, 1024, 20, "fcons6", 1, f_queue) );
    resume( create(future_prod, 1024, 20, "fprod3", 2, f_queue, (char*) &three) );
    resume( create(future_prod, 1024, 20, "fprod4", 2, f_queue, (char*) &four) );
    resume( create(future_prod, 1024, 20, "fprod5", 2, f_queue, (char*) &five) );
    resume( create(future_prod, 1024, 20, "fprod6", 2, f_queue, (char*) &six) );
    resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
    resume( create(future_cons, 1024, 20, "fcons8", 1, f_queue) );
    resume( create(future_cons, 1024, 20, "fcons9", 1, f_queue) );
}



/*void futureq_test (int nargs, char *args[]) {
  int three = 3, four = 4, five = 5, six = 6, seven = 7, eight = 8, nine=9, ten = 10, eleven = 11;

    future_t *f_queue;

    f_queue = future_alloc(FUTURE_QUEUE, sizeof(int), 3);

    // Test FUTURE_QUEUE Part1 
    resume(create(future_cons, 1024, 20, "fcons6", 1, f_queue));
    resume(create(future_cons, 1024, 20, "fcons7", 1, f_queue));
    resume(create(future_cons, 1024, 20, "fcons8", 1, f_queue));
    resume(create(future_cons, 1024, 20, "fcons9", 1, f_queue));
    resume(create(future_prod, 1024, 20, "fprod3", 2, f_queue, (char *)&three));
    resume(create(future_prod, 1024, 20, "fprod4", 2, f_queue, (char *)&four));
    resume(create(future_prod, 1024, 20, "fprod5", 2, f_queue, (char *)&five));
    resume(create(future_prod, 1024, 20, "fprod6", 2, f_queue, (char *)&six));

   // waitFor(100);
    future_free(f_queue);
    f_queue = future_alloc(FUTURE_QUEUE, sizeof(int), 3);
   // waitFor(100);
    // Test FUTURE_QUEUE Part2 
    resume(create(future_prod, 1024, 20, "fprod10", 2, f_queue, (char *)&seven));
    resume(create(future_prod, 1024, 20, "fprod11", 2, f_queue, (char *)&eight));
    resume(create(future_prod, 1024, 20, "fprod12", 2, f_queue, (char *)&nine));
    resume(create(future_prod, 1024, 20, "fprod13", 2, f_queue, (char *)&ten));
    resume(create(future_prod, 1024, 20, "fprod13", 2, f_queue, (char *)&eleven));

    resume(create(future_cons, 1024, 20, "fcons14", 1, f_queue));
    resume(create(future_cons, 1024, 20, "fcons15", 1, f_queue));
    resume(create(future_cons, 1024, 20, "fcons16", 1, f_queue));
    resume(create(future_cons, 1024, 20, "fcons17", 1, f_queue));
    resume(create(future_cons, 1024, 20, "fcons18", 1, f_queue));

   // waitFor(100);
    future_free(f_queue);
    f_queue = future_alloc(FUTURE_QUEUE, sizeof(int), 3);
   // waitFor(100);

    // Test FUTURE_QUEUE Part3 
    resume( create(future_cons, 1024, 20, "fcons6", 1, f_queue) );
    resume( create(future_prod, 1024, 20, "fprod3", 2, f_queue, (char*) &three) );
    resume( create(future_prod, 1024, 20, "fprod4", 2, f_queue, (char*) &four) );
    resume( create(future_prod, 1024, 20, "fprod5", 2, f_queue, (char*) &five) );
    resume( create(future_prod, 1024, 20, "fprod6", 2, f_queue, (char*) &six) );
    resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
    resume( create(future_cons, 1024, 20, "fcons8", 1, f_queue) );
    resume( create(future_cons, 1024, 20, "fcons9", 1, f_queue) );
    future_free(f_queue);
}
  
 */ 
 

shellcmd xsh_run(int nargs, char *args[])
{
  //Argument verifications and validations	
	if(nargs <= 1)
		printf("Too few arguments");
	else
	{	
		if(strncmp(args[1], "prodcons_bb",11) == 0)
		{
			args++;
			nargs--;
			resume(create(prodcons_bb, 4096, 20, "prodcons_bb", 2, nargs, args));
		}
		
		else if(strncmp(args[1], "futures_test", 12) == 0)
		{


			  if(strncmp(args[2],"-pc",3)==0){
			args++;
			nargs--;
				resume(create(futures_test, 4096, 20, "futures_test", 2, nargs, args));
			}
      
      
      	else  if (strncmp(args[2],"-fq",3)==0)
			{
			
        if (strncmp(args[2],"-fq1",4)==0)
        {
          /* code */
          args++;
				nargs--;
       
        resume(create(futureq_test1,4096,20,"future_queue1",2,nargs,args));

        }
        else if(strncmp(args[2],"-fq2",4)==0)
        {
          /* code */
          	args++;
				nargs--;
				resume(create(futureq_test2,4096,20,"future_queue2",2,nargs,args));
        }
        else 
        {
          /* code */
          	args++;
				nargs--;
				resume(create(futureq_test3,4096,20,"future_queue3",2,nargs,args));
        }
        
        
				
      }

      
			else{
				args++;
				nargs--;
				resume(create(future_fib, 4096, 20, "future_fib", 2, nargs, args));
			}
			
			
				
		}

     else if(strncmp(args[1], "fstest",6) == 0)
		{
			args++;
			nargs--;
			resume(create(fstest, 4096, 20, "fstest", 2, nargs, args));
		}
    	else if(strncmp(args[1],"tscdf_fq",7)==0)
	{
		args++;
		nargs--;
		
		resume(create(tscdf_fq,4096,20,"tscdf_fq",2,nargs,args));
		
	}

		else if(strncmp(args[1],"tscdf", 5)==0)
	{
		args++;
		nargs--;
		resume(create(tscdf,4096,20,"tscdf",2,nargs,args));
		
	}

    else if(strncmp(args[1],"rbt",3)==0){
      args++;
      nargs--;
      tr_TreeCreate();

      resume(create(callprocess,4096,20,"callprocess",0,0));
      resume(create(callprocess,4096,20,"callprocess",0,0));
      resume(create(callprocess,4096,20,"callprocess",0,0));
      resume(create(callprocess,4096,20,"callprocess",0,0));
    }
  


	}
	
	
  return (0);

}




void prodcons_bb(int nargs, char *args[]) {

  //create and initialize semaphores to necessary values
	full = semcreate(0);
	mutex=semcreate(1);
	empty=semcreate(5);
	
  //initialize read and write indices for the queue
	reads=0;
	writes=0;
	
  	int count =atoi(args[3]);
	int count1= atoi(args[4]);

  //create producer and consumer processes and put them in ready queue
	for (int j=0;j< atoi(args[1]);j++)
	{
	 resume( create(producer_bb, 1024, 20, "producer_bb", 2, count,j));
	}

	for(int k=0;k< atoi(args[2]);k++)
	{
	
  	 resume( create(consumer_bb, 1024, 20, "consumer_bb", 2,count1,k));
	}

  return (0);
}
	






