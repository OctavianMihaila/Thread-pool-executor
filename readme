# ThreadPoolExecutor

ThreadPoolExecutor is a thread scheduler that efficiently manages and allocates tasks to available threads for execution, enabling parallel task execution.

## The Running Process

### Initialization

- ThreadPoolExecutor is initialized with two attributes from the input file:
   1. A time quantum `Q` measured in milliseconds (ms), representing the maximum continuous running time of a thread.
   2. The number of available CPU cores `C`, from which the maximum number of threads that can run in parallel is derived as `N = 2 * C`.

- The available thread pool is created based on `N`, and task queues are initialized.

### Running Tasks in Parallel

- Tasks from the queue are executed until the queue is empty or there are no available threads in the pool.

- The execution process occurs in cycles with a time step of `Q`. Tasks are executed for a maximum of `Q` time at a time.

- After each cycle, ThreadPoolExecutor performs checks to determine whether tasks are available for execution. The execution process continues in a cyclic manner.

## Implementation

- The implementation of ThreadPoolExecutor is based on stacks and queues using generic lists.

- Data structures and their related functions are described in the files `queue.h` and `stack.h`, with specific operations found in `QueueFunctions.c` and `StackFunctions.c`.
