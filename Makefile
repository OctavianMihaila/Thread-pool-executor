build: 
	gcc -g -Wall -m32 -std=c99 -o threadpoolexecutor ThreadPoolExecutor.c functions.c QueueFunctions.c StackFunctions.c
run:
	./threadpoolexecutor
clean:
	rm -f *.o threadpoolexecutor
