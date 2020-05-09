
#include <xinu.h>
#include<string.h>
#include<stdio.h>
#include <stdlib.h>
#include<streamproc.h>
#include <tscdf_fqinput.h>
#include "tscdf.h"

static int pointer_array[2];
int work_queue_depths=10;
int num_futures=10;
int time_windows=10;
int output_times=10;
int st;
int ts;
int v;
uint pcport;




void stream_consumer_future(int32 id, future *f){

   int count=0;
    char output[256];
    struct tscdf *tc = tscdf_init(time_windows);
    int ltime=0;
    int lvalue=0;
     kprintf("stream_consumer_future id:%d (pid:%d)\n",id,getpid());
      while(1){

        if (f->count == 0){
                  enqueue(getpid(), f->get_queue);
                  suspend(getpid());
                  }
        ltime = f->queue[f->tail].time;
        lvalue = f->queue[f->tail].value;
       tscdf_update(tc, f->queue[f->tail].time , f->queue[f->tail].value);
        f->tail = (f->tail + 1) % work_queue_depths ;
        f->count = f->count - 1;
       if(ltime==0 ){
                    tscdf_free(tc);
                            break;
                            }

              count++;

     if(count % output_times == 0){
     
     int32 *qarray = tscdf_quartiles(tc);

          if(qarray == NULL) {
            kprintf("tscdf_quartiles returned NULL\n");
            continue;
          }

          sprintf(output, "s%d: %d %d %d %d %d", id, qarray[0], qarray[1], qarray[2], qarray[3], qarray[4]);
          kprintf("%s\n", output);

         // freemem((char *)qarray, (6*sizeof(int32)));
          }


         }

      ptsend(pcport, getpid());



}




int stream_proc_futures(int nargs, char* args[]) {
  // Parse arguments

   int i;
    char usage[] = "Usage: -s num_streams -w work_queue_depth -t time_window -o output_time\n";
    char *ch;
    char c;
    ulong secs, msecs, time;
    secs = clktime;
    msecs = clkticks;

  // Create array to hold `n_streams` number of futures
  //future_t **farray;
 
  // Create consumer processes and allocate futures
  // Use `i` as the stream id.
  // Future mode = FUTURE_QUEUE
  // Size of element = sizeof(struct data_element)
  // Number of elements = work_queue_depth

  // Parse input header file data and set future values

  if (!(nargs % 2)) {
      printf("%s", usage);
      return(-1);
    }
    else {
      i = nargs - 1;
      while (i > 0) {
        ch = args[i-1];
        c = *(++ch);

        switch(c) {
        case 's':
          num_futures = atoi(args[i]);
           
          break;

        case 'w':
          work_queue_depths = atoi(args[i]);
          break;

        case 't':
          time_windows = atoi(args[i]);
          break;

        case 'o':
          output_times = atoi(args[i]);
          break;

        default:
          printf("%s", usage);
          return(-1);
        }

        i -= 2;
      }
      }

 
      if((pcport = ptcreate(num_futures)) == SYSERR) {
          printf("ptcreate failed\n");
          return(-1);
      }


      future *farray;
    if((farray = (future *)getmem(sizeof(future)*(work_queue_depths))) == (future *) SYSERR){
              printf("NOTT WORKINGGG");
              return (future *)SYSERR;
              }
      for(i=0; i<num_futures; i++){
      farray[i].tail = 0;
      farray[i].head = 0;
      farray[i].mode = FUTURE_QUEUE;
       farray[i].count=0;
       farray[i].nelems =  work_queue_depths;
       farray[i].set_queue = newqueue();
       farray[i].get_queue = newqueue();
      farray[i].queue = (des *)getmem(sizeof(des) *work_queue_depths);



      }


for (i = 0; i < num_futures; i++) {
        resume(create(stream_consumer_future, 4096,20, "stream_consumer_future", 2, i,  &farray[i]));
  }


  char *a;
  
  for(i=0;i<n_inputs;i++){
    
     a = (char *)future_input[i];
        st = atoi(a);
        while (*a++ != '\t');
        ts = atoi(a);
        while (*a++ != '\t');
        v = atoi(a);


            farray[st].queue[farray[st].head].time = ts;

            farray[st].queue[farray[st].head].value = v;
           // kprintf("\n Produer setting the data : %d %d\n", farray[st].queue[farray[st].head].time ,farray[st].queue[farray[st].head].value );
            farray[st].count = farray[st].count + 1;
             farray[st].head = ((farray[st].head +1)% work_queue_depths);
             pid32 p = dequeue(farray[st].get_queue);
             if(p != -1){
                             resume(p);
                         }

        
        }


  

    for(i=0; i < num_futures; i++) {
        uint32 pm;
        pm = ptrecv(pcport);
        kprintf("process %d exited\n", pm);

    }

    ptdelete(pcport, 0);

time = (((clktime * 1000) + clkticks) - ((secs * 1000) + msecs));
kprintf("time in ms: %u\n", time);


  return 0;
}






