// declare globally shared array
extern int arr_q[5];


// declare globally shared semaphores
extern sid32 full,  mutex, empty;

// declare globally shared read and write indices
extern int reads, writes; 

// function prototypes
void consumer_bb(int count, int pid);
void producer_bb(int count, int pid);