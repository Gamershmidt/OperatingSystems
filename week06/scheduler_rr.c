#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#define PS_MAX 10




// holds the scheduling data of one process
typedef struct{
	int idx; // process idx (index)
	int at, bt, rt, wt, ct, tat; // arrival time, burst time, response time, waiting time, completion time, turnaround time.
	int burst; // remaining burst (this should decrement when the process is being executed)
    int qtime;
} ProcessData;



// the idx of the running process
int running_process = -1; // -1 means no running processes

// the total time of the timer
unsigned total_time; // should increment one second at a time by the scheduler

// data of the processes
ProcessData data[PS_MAX]; // array of process data

// array of all process pids
pid_t ps[PS_MAX]; // zero valued pids - means the process is terminated or not created yet

// size of data array
unsigned data_size;

int qtm;

int queue[PS_MAX];
int begin = 0, end = 0;

void update() {
    for(int i=0; i < data_size; i++) {
        if(data[i].at == total_time) {
            printf("%d\n", i);
            queue[end] = data[i].idx;
            end++;
            end = end%PS_MAX;
        }
    }
}

void read_file(FILE* file) {
    for (int i = 0; i < PS_MAX; i++) {
        ps[i] = 0;
    }

    if (file == NULL) {
        printf("Error: Unable to open the file.\n");
        return;
    }

    int process_id, arrival_time, burst_time;
    data_size = 0; 
    //fscanf(file, "%d %d %d", &process_id, &arrival_time, &burst_time)
    while (fscanf(file, "%d", &process_id) != EOF) {
        fscanf(file, "%d", &arrival_time);
        fscanf(file, "%d",&burst_time);
        if (data_size < PS_MAX) {
            data[data_size].idx = process_id;
            data[data_size].at = arrival_time;
            data[data_size].bt = burst_time;
            data[data_size].rt = burst_time; 
            data[data_size].burst = burst_time;
            data[data_size].qtime = qtm;
            data_size++;

            
        } else {
            printf("Error: Maximum number of processes reached.\n");
        }
    }
    fclose(file);
}

// send signal SIGCONT to the worker process
void resume(pid_t process) {
    if (kill(process, 0) == 0) {
        kill(process, SIGCONT);
    }
}

// send signal SIGTSTP to the worker process
void suspend(pid_t process) {
    sleep(1);
    if (kill(process, 0) == 0) {
        kill(process, SIGTSTP);
    }
}

// send signal SIGTERM to the worker process
void terminate(pid_t process) {
    if (kill(process, 0) == 0) {
        kill(process, SIGTERM);
    }
}

// create a process using fork
void create_process(int new_process) {
    if (running_process != -1)
        suspend(ps[running_process]);
    pid_t child_pid = fork();
    if (child_pid == 0) {
        char process_id_str[16];
        sprintf(process_id_str, "%d", new_process);
        char* args[] = {"./worker", process_id_str, NULL};
        execvp(args[0], args);
    } else {
        ps[new_process] = child_pid;
        running_process = new_process;
    }
}

// find next process for running
ProcessData find_next_process() {
  // location of next process in {data} array
    
	int location = 0;
    if(data[running_process].burst > 0 && data[running_process].qtime == 0) {
        queue[end] = running_process;
        data[running_process].qtime = qtm;
        end++;
        end %= PS_MAX;
    }
    location = queue[begin];
    
    if(data[location].at > total_time){
        
        printf("Scheduler: Runtime: %u seconds.\nProcess %d: has not arrived yet.\n", total_time, location);
        
        // increment the time
        total_time++;
        update();
        return find_next_process(); 
	}
    begin++;
    begin %= PS_MAX;

  // return the data of next process
	return data[location];
}


// reports the metrics and simulation results
void report(){
	printf("Simulation results.....\n");
	int sum_wt = 0;
	int sum_tat = 0;
	for (int i=0; i< data_size; i++){
		printf("process %d: \n", i);
		printf("	at=%d\n", data[i].at);
		printf("	bt=%d\n", data[i].bt);
		printf("	ct=%d\n", data[i].ct);
		printf("	wt=%d\n", data[i].wt);
		printf("	tat=%d\n", data[i].tat);
		printf("	rt=%d\n", data[i].rt);
		sum_wt += data[i].wt;
		sum_tat += data[i].tat;
	}

	printf("data size = %d\n", data_size);
	float avg_wt = (float)sum_wt/data_size;
	float avg_tat = (float)sum_tat/data_size;
	printf("Average results for this run:\n");
	printf("	avg_wt=%f\n", avg_wt);
	printf("	avg_tat=%f\n", avg_tat);
}

void check_burst(){

	for(int i = 0; i < data_size; i++)
		if (data[i].burst > 0)
		    return;

    // report simulation results
    report();

    // terminate the scheduler :)
	  exit(EXIT_SUCCESS);
}


// This function is called every one second as handler for SIGALRM signal
void schedule_handler(int signum) {
    // Increment the total time

    
    if (running_process != -1 && data[running_process].qtime > 0) {
        total_time++;
        update();
        data[running_process].burst--;
        data[running_process].qtime--;
        printf("Scheduler: Runtime: %u seconds\n", total_time);
        if (data[running_process].burst <= 0) {
            printf("Scheduler: Terminating Process %d (Remaining Time: 0)\n", running_process);
            terminate(ps[running_process]);
            data[running_process].tat = total_time - data[running_process].at;
            data[running_process].wt = data[running_process].tat - data[running_process].bt;
            data[running_process].ct = total_time;
            running_process = -1;
        }
        if (data[running_process].qtime == 0) schedule_handler(signum);   
        printf("Process %d is running with %d seconds left\n", running_process, data[running_process].burst);
    } else {
        check_burst();
        ProcessData next_process = find_next_process();
        if (running_process != -1) {
            data[running_process].qtime = qtm;
            printf("Scheduler: Stopping Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst); 
            suspend(ps[running_process]);  
        }
        if(ps[next_process.idx] == 0) {
               create_process(next_process.idx); 
               running_process = next_process.idx;
               data[running_process].rt =total_time - data[running_process].at;
               printf("Scheduler: Starting Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
        } else{
          resume(ps[next_process.idx]);
          running_process = next_process.idx;
          printf("Scheduler: Resuming Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
        }
    }
    
    
}


int main(int argc, char *argv[]) {
    printf("Enter time quantum:");
    //int time_quantum1;
    scanf("%d", &qtm);
    for (int i= 0;i < 10000000;i++);
    // read the data file
    FILE *in_file  = fopen(argv[1], "r");
  	if (in_file == NULL) {   
		printf("File is not found or cannot open it!\n"); 
  		exit(EXIT_FAILURE);
    } else {
        read_file(in_file);  
    }
    
    
    // set a  timer
    struct itimerval timer;

    // the timer goes off 1 second after reset
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;

    // timer increments 1 second at a time
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    // this counts down and sends SIGALRM to the scheduler process after expiration.
    setitimer(ITIMER_REAL, &timer, NULL);
    

    // register the handler for SIGALRM signal
    signal(SIGALRM, schedule_handler);

    // Wait till all processes finish
    while(1); // infinite loop
}
