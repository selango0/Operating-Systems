/**

CS341 
Shalmika Sougandini
PA 2: Real-Time Event Scheduler


This program reads in user input and stores event requests in any order and then executes the requests in order in real time. I combined the given minHeap (alternated a bit) with my scheduler in one file main.c. I struggled a bit with this assignment but doing it in one .c file with the heap + thread made the most sense to me, although it may not be appropriate. 

Run instructions: 

$make
$./main inp.txt

The initial -1 is required to start the timer and initialize second thread. I made it to wait for initial -1 so that you can easily put all your inputs and then the output is generated. 

You can uncomment the print statements in my code to better understand the program. It was helpful in debugging. 

**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
#include <time.h>
#include <string.h>

#define max 100
//global variable
pthread_t thread_id2;
long arr[max];
int size = 0;
long startTime;
int heapBusy = 0;
char* init_d;
int done = 0;

struct process{
    int timestamp[max];
    long absTS[max];
    char name[max][20];
};

struct process p;

/************************** HEAP CODE ***************************************/

void swap(int a, int b){
	//printf("in swap\n");
    int temp1;
    long temp2;
    char n[20];

    temp1 = p.timestamp[a];
    p.timestamp[a] = p.timestamp[b];
    p.timestamp[b] = temp1;

    temp2 = p.absTS[a];
    p.absTS[a] = p.absTS[b];
    p.absTS[b] = temp2;

    strcpy(n,p.name[a]);
    strcpy(p.name[a],p.name[b]);
    strcpy(p.name[b],n);
}

void heapify(long arr[], int n, int i){

	//printf("in heapify\n");
	int largest = i; // Initialize largest as root
	int l = 2*i; // left = 2*i + 1
	int r = 2*i + 1; // right = 2*i + 2

	// If left child is larger than root
	if (l < n && p.absTS[l] > p.absTS[largest]) //arr[l] > arr[largest])
		largest = l;

	// If right child is larger than largest so far
	if (r < n && p.absTS[r] > p.absTS[largest]) // arr[r] > arr[largest])
		largest = r;

	// If largest is not root
	if (largest != i)
	{
		swap(i,largest);

		// Recursively heapify the affected sub-tree
		heapify(arr, n, largest);
	}
}

// main function to do heap sort
void heapSort(long arr[], int n){

	//printf("in heapSort\n");
	// Build heap (rearrange array)
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(arr, n, i);

	// One by one extract an element from heap
	for (int i=n-1; i>=0; i--)
	{
		// Move current root to end
		swap(0,i);

		// call max heapify on the reduced heap
		heapify(arr, i, 0);
	}
}

void insert(int ts,long key ,char names[]){
	//printf("in insert\n");
    while(heapBusy);
    heapBusy =1;

    p.timestamp[size] = ts;
    p.absTS[size] = key;
    strcpy(p.name[size], names); 

    size = size + 1;
    heapSort(arr, size);

    heapBusy =0;
}

long returnMin(){
	//printf("in returnMin\n");
    while(heapBusy);
    return p.absTS[0];
}

long deleteMin(){
	//printf("in deleteMin\n");
    while(heapBusy);
    heapBusy =1;
    long t = p.absTS[0];
    p.timestamp[0] = p.timestamp[size-1];
    p.absTS[0] = p.absTS[size-1];
    strcpy(p.name[0],p.name[size-1]);
    size = size - 1;
    heapSort(arr,size);

    heapBusy =0;
    
    return t;
}

/* A utility function to print array of size n */
void printHeap()
{
	for (int i=0; i<size; ++i)
		printf("%ld ",p.absTS[i]);
	printf("\n");
}



/************************** THREAD CODE ***************************************/

void *clockThread(void *vargp) {
	//printf("in clockThread");
    printf("\n\n Output:\n");
    //printf("before while, size = %d\n", size);
    while(size>0){
    	//printf("in while, size = %d\n", size);
    	//printf("while(heapBusy); starts\n");
        while(heapBusy);
        //printf("while(heapBusy); ends\n");
        if(size>0){
        	//printf("inside if, size  =  %d\n", size);
            if(returnMin() <= time(0)){
            //	printf("a process has matured, printing the  process.\n");
                printf("%d\t",p.timestamp[0]);
                puts(p.name[0]);
            
                long removed = deleteMin();
          //      printf("deleted the process successfully\n");
            }
        }
        //printf("going to sleep\n");
        sleep(1);
      //  printf("awake from sleep\n\n\n\n");
    }
    //printf("after while, size = %d\n", size);
    printf("all process complete.\n");
    done = 1;
	//exit(1);
    return NULL;
}

void *inputThread(void *vargp) {
	//printf("in inputThread\n");
    int input;
    char name[20];
    long st = time(0);

    //file reading starts
    FILE *fr;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    char ch[10];
    char words[100];

    fr = fopen(init_d, "r"); // read mode

    while ((read = getline(&line, &len, fr)) != -1) {
    	//printf("Inside while inputThread\n");
        printf("input -> %s", line);

        char *space_ptr = strchr(line, ' ');
        int posn = -1;
        if (space_ptr != NULL){
            posn = space_ptr - line;
        }

        int c = 0;
        while (c < posn) {
            ch[c] = line[c];
            c++;
        }
        ch[c] = '\0';

        c = posn;
        int ind = 0;
        while (c  < (int) read) {
            words[ind++] = line[c];
            c++;
        }
        words[ind] = '\0';

        int num;
        sscanf(ch, "%d", &num);

        long timeDelay = num + st;    //Step up the difference from clock delay
        insert(num,timeDelay,words);
    }
    //printf("\ndone with input, exiting inputThread\n");
    fclose(fr);
    if (line)
        free(line);
    //exit(EXIT_SUCCESS);
    startTime = time(0);

    //printf("\ncreating thread 2\n");
    pthread_create(&thread_id2, NULL, clockThread, NULL);

    return NULL;
}


int main(int argc, char *argv[])
{
	init_d = argv[1];
    printf("%s\n", init_d);
    pthread_t thread_id, thread_id2;
    printf("Before Thread\n");
    pthread_create(&thread_id, NULL, inputThread, NULL);

    //printf("waiting for join start");
    pthread_join(thread_id, NULL);
   // printf("\nwaiting for join start, 1 complete\n");
    pthread_join(thread_id2, NULL);
    //printf("\nwaiting for join start, both complete\n");
    while(!done){
    	sleep(1);
    }
   // exit(1);
    printf("main complete\n");

    return 0;
}
