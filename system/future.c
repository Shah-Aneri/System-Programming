#include <xinu.h>
#include <stdio.h>
#include <future.h>



// Allocating memory for the future.
future_t* future_alloc(future_mode_t mode, uint size, uint nelems)
{
    future_t *fut;

    if ((fut = (future_t *)getmem(sizeof(future_t) + (size))) == (future_t *)SYSERR)
    {
       
        return (future_t *)(SYSERR);
    }

    fut->mode = mode;
    fut->data = sizeof(future_t) + (char *)fut;
    fut->state = FUTURE_EMPTY;
    if (mode == FUTURE_SHARED)
    {
		//fut->data = sizeof(future_t) + (char *)fut;
        fut->set_queue = newqueue();
        fut->get_queue = newqueue();
    }

	else if (mode == FUTURE_QUEUE){
		fut->set_queue = newqueue();
        fut->get_queue = newqueue();
		fut->max_elems = nelems;
		fut-> count =0;
		fut-> head=0;
		fut->tail=0;
		fut->data =  (future_t *)getmem(sizeof(future_t)*(nelems) );
		

	}
	

    return fut;
}

// Freeing the memory for the future.
syscall future_free(future_t *f)
{
    if (freemem(f, sizeof(f) + f->size) == (future_t *)SYSERR)
    {
        return (SYSERR);
    }
    else
    {
        return (OK);
    }
}

/* Setting the value of future from the given value.*/
syscall future_set(future_t* f, char* value){
	int i;
	intmask mask;
	mask = disable();

	
	

	/* FUTURE_SHARED */

	if(f->mode == FUTURE_SHARED) {
		if(f->state == FUTURE_READY){
			return SYSERR;
		}
		else if(f->state == FUTURE_WAITING) {
			/* set the future value and state */
			memcpy(f->data, value, sizeof(value));
			f->state = FUTURE_READY;

			/* dequeue all processes waiting */
			pid32 p;
            while ((p = dequeue(f->get_queue)) != EMPTY)
            {
                resume(p);
            }
		}
		else if(f->state == FUTURE_EMPTY){ 
			/* set the future value and state */
			memcpy(f->data, value, sizeof(value));
			f->state = FUTURE_READY;
		}
	} 

	
/*************************************************************** 
Future Queue 
***************************************************************/


	else if(f->mode == FUTURE_QUEUE)
    {
      
        if(f->count == f->max_elems){
			
            restore(mask);
            enqueue(getpid(),f->set_queue);
			f->state = FUTURE_WAITING;
            suspend(getpid());    //suspend process
		     memcpy(f->data[f->tail], value, sizeof(value));
			 f->tail = (f->tail + 1) % f->max_elems;
			 f->count = f->count+1;
						
			
			
		}else if (f->get_queue!=NULL){
			
			 memcpy(f->data[f->tail], value, sizeof(value));
			f->tail = (f->tail + 1) % f->max_elems;
			f->count = f->count+1;
			f->state=FUTURE_READY;
			pid32 p;
			if ((p = dequeue(f->get_queue)) != EMPTY)
            {
				
                resume(p);
				
            }
			

		}   
    }
	/*------------------------------------------------------------------------
	 *  EXCLUSIVE mode
	 *------------------------------------------------------------------------
	 */
	else {
		
		if(f->state == FUTURE_READY){		
			return SYSERR;
		}
		else if(f->state == FUTURE_WAITING){
			
			memcpy(f->data, value, sizeof(value));		
			f->state = FUTURE_READY;
			resume(f->pid);				
		}
		else if(f->state == FUTURE_EMPTY){
			/* set the future value and state */
			memcpy(f->data, value, sizeof(value));
			f->state = FUTURE_READY;
		}
	} 
	
	
	restore(mask);
	return OK;

}

/* Getting the value from future.*/
syscall future_get(future_t* f, char* value){
	intmask mask;
	mask = disable();
	
	/* FUTURE_SHARED */

	if(f->mode == FUTURE_SHARED) {
		
		if(f->state == FUTURE_WAITING){
			/* enqueue the current process in get_queue  */
			 enqueue(getpid(), f->get_queue);
        	 suspend(getpid());
		}
		else if(f->state == FUTURE_EMPTY){
			/* enqueue the current process in get_queue  */
			f->state = FUTURE_WAITING;
			enqueue(getpid(), f->get_queue);
			suspend(getpid());
			memcpy(value, f->data, sizeof(f->data));
            return (OK);
		}
		
		if(f->state == FUTURE_READY){
			memcpy(value, f->data, sizeof(f->data));
		}
		restore(mask);
	}

/********************************
 * Future_Queue
 * *****************************/
	

	else if(f->mode == FUTURE_QUEUE)
    {
       
        
        if(f->count == 0){
            restore(mask);
            enqueue(getpid(),f->get_queue);
			 f->state = FUTURE_WAITING;
            suspend(getpid());    //suspend process
			 memcpy( value,f->data[f->head], sizeof(f->data[f->head]));
			 f->head = (f->head + 1) % f->max_elems;
			 f->count = f->count-1;
					
           
			
		}else if (f->set_queue!=NULL)
		
		{
			
			 memcpy( value,f->data[f->head], sizeof(f->data[f->head]));
			 	f->head = (f->head + 1) % f->max_elems;
				 f->count = f->count-1;
			pid32 p;
			
		
			if((p = dequeue(f->set_queue)) != EMPTY)
            {
				
                resume(p);
            }

			

		}   
    }




	// FUTURE_EXCLUSIVE MODE
	else {
		if(f->state == FUTURE_READY){
			memcpy(value, f->data, sizeof(f->data));
			f->state = FUTURE_EMPTY;
		}
		else if(f->state == FUTURE_WAITING){
			return SYSERR;
		}
		else if(f->state == FUTURE_EMPTY){ 
			/*block waiting for it to set and store pid */
			f->state = FUTURE_WAITING;
			f->pid = getpid();
			suspend(getpid());
			memcpy(value, f->data, sizeof(f->data));
		}
	} 


	restore(mask);
	return OK;
}

















































