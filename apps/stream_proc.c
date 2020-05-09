#include <xinu.h>
#include <stream.h>
#include<string.h>
#include<stdio.h>
#include <stdlib.h>
#include"tscdf.h"


static int pointer_array[2];
int work_queue_depth=10;
int num_streams=10;
int time_window=10;
int output_time=10;
int st;
int ts;
int v;
uint pcport;

void stream_consumer(int32 id, struct stream *str ){

  
    int count=0;
    char output[256];
    struct tscdf *tc = tscdf_init(time_window);
    int ltime=0;
    int lvalue=0;
    kprintf("stream_consumer id:%d (pid:%d)\n",id,getpid());
      while(1){
        wait(str->spaces);
        wait(str->mutex);
        ltime = str->queue[str->tail].time;
        lvalue = str->queue[str->tail].value;
        tscdf_update(tc, str->queue[str->tail].time , str->queue[str->tail].value);
        str->tail = (str->tail + 1) % work_queue_depth ;
       signal(str->mutex);
       signal(str->items);
       if(ltime==0 ){
                    tscdf_free(tc);
                    kprintf("stream_consumer exiting\n");
                            break;
                            }

              count++;
     if(count % output_time == 0){
     
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



int stream_proc(int nargs, char* args[]) {

  // Parse arguments
    int i;
    char usage[] = "Usage: -s num_streams -w work_queue_depth -t time_window -o output_time\n";
    char *ch;
    char c;
    ulong secs, msecs, time;
    secs = clktime;
    msecs = clkticks;
    /* Parse arguments out of flags */
    /* if not even # args, print error and exit */
   
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
          num_streams = atoi(args[i]);
           
          break;

        case 'w':
          work_queue_depth = atoi(args[i]);
          break;

        case 't':
          time_window = atoi(args[i]);
          break;

        case 'o':
          output_time = atoi(args[i]);
          break;

        default:
          printf("%s", usage);
          return(-1);
        }

        i -= 2;
      }
      }

 
      if((pcport = ptcreate(num_streams)) == SYSERR) {
          printf("ptcreate failed\n");
          return(-1);
      }

  // Create streams
  stream *stream1;
    if((stream1 = (stream *)getmem(sizeof(stream)*(work_queue_depth))) == (stream *) SYSERR){
              printf("NOTT WORKINGGG");
              return (stream *)SYSERR;
              }
      for(i=0; i<num_streams; i++){
      stream1[i].tail = 0;
      stream1[i].head = 0;
      stream1[i].items = semcreate(work_queue_depth);
      stream1[i].mutex = semcreate(1);
      stream1[i].spaces = semcreate(0);
      stream1[i].queue = (de *)getmem(sizeof(de) *work_queue_depth);
//      (stream1[i].queue)->time = -1;
//      (stream1[i].queue)->value = -1;
      }

      
  // Create consumer processes and initialize streams
  // Use `i` as the stream id.
  for (i = 0; i < num_streams; i++) {
        resume(create(stream_consumer, 4096,20, "stream_consumer", 2, i,  &stream1[i]));
  }



  // Parse input header file data and populate work queue
  char *a;
  
  for(i=0;i<n_input;i++){
    
     a = (char *)stream_input[i];
        st = atoi(a);
        while (*a++ != '\t');
        ts = atoi(a);
        while (*a++ != '\t');
        v = atoi(a);

          wait(stream1[st].items);
             wait(stream1[st].mutex);

            stream1[st].queue[stream1[st].head].time = ts;
            stream1[st].queue[stream1[st].head].value = v;

             stream1[st].head = ((stream1[st].head +1)% work_queue_depth);
        
            signal(stream1[st].mutex);
            signal(stream1[st].spaces);
        
        }


  

    for(i=0; i < num_streams; i++) {
        uint32 pm;
        pm = ptrecv(pcport);
        kprintf("process %d exited\n", pm);

    }

    ptdelete(pcport, 0);

time = (((clktime * 1000) + clkticks) - ((secs * 1000) + msecs));
kprintf("time in ms: %u\n", time);


  return 0;
}


