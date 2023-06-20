# posix_Semaphore


# Thread Basics:

    Thread operations include thread creation, termination, synchronization (joins,blocking), scheduling, data management and process interaction.
    A thread does not maintain a list of created threads, nor does it know the thread that created it.
    All threads within a process share the same address space.
    *   Threads in the same process share:
        -Process instructions
        -Most data
        -open files (descriptors)
        -signals and signal handlers
        -current working directory
        -User and group id
    *   Each thread has a unique:
        -Thread ID
        -set of registers, stack pointer
        -stack for local variables, return addresses
        -signal mask
        -priority
        -Return value: errno
    *   pthread functions return "0" if OK.

# Thread Synchronization:

The threads library provides three synchronization mechanisms:

    mutexes - Mutual exclusion lock: Block access to variables by other threads. This enforces exclusive access by a thread to a variable or set of variables.
    joins - Make a thread wait till others are complete (terminated).
    condition variables - data type pthread_cond_t
# Mutexes:
Mutexes are used to prevent data inconsistencies due to race conditions. A race condition often occurs when two or more threads need to perform operations on the same memory area, but the results of computations depends on the order in which these operations are performed. Mutexes are used for serializing shared resources. Anytime a global resource is accessed by more than one thread the resource should have a Mutex associated with it. One can apply a mutex to protect a segment of memory ("critical region") from other threads. Mutexes can be applied only to threads in a single process and do not work between processes as do semaphores.

Example threaded function: 

    /* Note scope of variable and mutex are the same */
    pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
    int counter=0;

    /* Function C */
    void functionC()
    {
    pthread_mutex_lock( &mutex1 );
    counter++
    pthread_mutex_unlock( &mutex1 );
    }

# Joins:
A join is performed when one wants to wait for a thread to finish. A thread calling routine may launch multiple threads then wait for them to finish to get the results. One wait for the completion of the threads with a join. 

# Condition Variables:

A condition variable is a variable of type pthread_cond_t and is used with the appropriate functions for waiting and later, process continuation. The condition variable mechanism allows threads to suspend execution and relinquish the processor until some condition is true. A condition variable must always be associated with a mutex to avoid a race condition created by one thread preparing to wait and another thread which may signal the condition before the first thread actually waits on it resulting in a deadlock. The thread will be perpetually waiting for a signal that is never sent. Any mutex can be used, there is no explicit link between the mutex and the condition variable.

Functions used in conjunction with the condition variable:

    *   Creating/Destroying:
        pthread_cond_init
        pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
        pthread_cond_destroy
    *   Waiting on condition:
        pthread_cond_wait
        pthread_cond_timedwait - place limit on how long it will block.
    *   Waking thread based on condition:
        pthread_cond_signal
        pthread_cond_broadcast - wake up all threads blocked by the specified condition variable.

# Thread Scheduling:

When this option is enabled, each thread may have its own scheduling properties. Scheduling attributes may be specified:

    *   during thread creation
    *   by dynamically by changing the attributes of a thread already created
    *   by defining the effect of a mutex on the thread's scheduling when creating a mutex
    *   by dynamically changing the scheduling of a thread during synchronization operations.

The threads library provides default values that are sufficient for most cases. 

# Thread Pitfalls:

    *Race conditions: 
        While the code may appear on the screen in the order you wish the code to execute, threads are scheduled by the operating system and are executed at random. It cannot be assumed that threads are executed in the order they are created. They may also execute at different speeds. When threads are executing (racing to complete) they may give unexpected results (race condition). Mutexes and joins must be utilized to achieve a predictable execution order and outcome.

    *Thread safe code: 
        The threaded routines must call functions which are "thread safe". This means that there are no static or global variables which other threads may clobber or read assuming single threaded operation. If static or global variables are used then mutexes must be applied or the functions must be re-written to avoid the use of these variables. In C, local variables are dynamically allocated on the stack. Therefore, any function that does not use static data or other shared resources is thread-safe. Thread-unsafe functions may be used by only one thread at a time in a program and the uniqueness of the thread must be ensured. Many non-reentrant functions return a pointer to static data. This can be avoided by returning dynamically allocated data or using caller-provided storage. An example of a non-thread safe function is strtok which is also not re-entrant. The "thread safe" version is the re-entrant version strtok_r.

    *Mutex Deadlock: 
        This condition occurs when a mutex is applied but then not "unlocked". This causes program execution to halt indefinitely. It can also be caused by poor application of mutexes or joins. Be careful when applying two or more mutexes to a section of code. If the first pthread_mutex_lock is applied and the second pthread_mutex_lock fails due to another thread applying a mutex, the first mutex may eventually lock all other threads from accessing data including the thread which holds the second mutex. The threads may wait indefinitely for the resource to become free causing a deadlock. It is best to test and if failure occurs, free the resources and stall before retrying. 

# Help Resources:
https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html