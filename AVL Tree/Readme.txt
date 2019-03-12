CS341 
PA 2: Real-Time Event Scheduler

NOTE: ignore the warning when compiling. Continue exeucting ./main inp.txt
This program reads in user input and stores event requests in any order and then executes the requests in order in real time. I combined the given minHeap (alternated a bit) with my scheduler in one file main.c. I struggled a bit with this assignment but doing it in one .c file with the heap + thread made the most sense to me, although it may not be appropriate. 

Run instructions: 

$make
$./main inp.txt

The initial -1 is required to start the timer and initialize second thread. I made it to wait for initial -1 so that you can easily put all your inputs and then the output is generated. 

You can uncomment the print statements in my code to better understand what it is doing, if you would like.
It was helpful in debugging. 
