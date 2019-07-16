#include <ctime>
#include <cstdio>
#include <unistd.h>

void current_time(struct timespec *time){
    clock_gettime(CLOCK_MONOTONIC, time);
}

void time_diff(struct timespec * start, struct timespec * end, struct timespec * diff){
    if ((end->tv_nsec - start->tv_nsec) < 0) {
        diff->tv_sec = end->tv_sec-start->tv_sec-1;
        diff->tv_nsec = 1000000000+end->tv_nsec-start->tv_nsec;
    } else {
        diff->tv_sec = end->tv_sec-start->tv_sec;
        diff->tv_nsec = end->tv_nsec-start->tv_nsec;
    }
}

int main(){
    struct timespec startTime;
    struct timespec endTime;
    struct timespec diff;
    current_time(&startTime);
    usleep(2000*1000);
    current_time(&endTime);

    time_diff(&startTime, &endTime, &diff);
    double time = (double) diff.tv_sec + diff.tv_nsec*1.0e-9;

    printf("current time %lf (s)\n", time);
    return (int) time;
}
