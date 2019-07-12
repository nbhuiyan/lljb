#include <stdio.h>
#include <time.h>

typedef double FP_t;

__attribute__((noinline))
void mandelbrot(int x_pixels, int y_pixels, int* out_table, int max_iterations) {
    FP_t x_scale_factor = 3.5 / (FP_t) x_pixels;
    FP_t y_scale_factor = 2.0 / (FP_t) y_pixels;

    for (int py = 0; py < y_pixels; ++py) {
        for (int px = 0; px < x_pixels; ++px) {
            FP_t x0 = px * x_scale_factor - 2.5;
            FP_t y0 = py * y_scale_factor - 1.0;

            FP_t x = 0.0;
            FP_t y = 0.0;
            int iteration = 0;
            while (x*x + y*y <= 2*2 && iteration < max_iterations) {
                FP_t x_temp = x*x - y*y + x0;
                y = 2*x*y + y0;
                x = x_temp;
                iteration += 1;
            }

            out_table[py*x_pixels + px] = iteration;
        }
    }
}

void print_image(int x_pixels, int y_pixels, int* table) {
    for (int py = 0; py < y_pixels; ++py) {
       for (int px = 0; px < x_pixels; ++px) {
           char c = table[py*x_pixels + px] > 150 ? '*' : ' ';
           printf("%s",&c);
       }
       char c = '\n';
       printf("%s", &c);
    }
}

struct timespec tdiff(struct timespec * start, struct timespec * end) {
    struct timespec temp;
    if ((end->tv_nsec-start->tv_nsec)<0) {
        temp.tv_sec = end->tv_sec-start->tv_sec-1;
        temp.tv_nsec = 1000000000+end->tv_nsec-start->tv_nsec;
    } else {
        temp.tv_sec = end->tv_sec-start->tv_sec;
        temp.tv_nsec = end->tv_nsec-start->tv_nsec;
    }
    return temp;
}

int main(void) {
    int small_table [3][4] = {0};
    mandelbrot(4, 3, (int*)small_table, 10);
    print_image(4, 3, (int*)small_table);

    int table[34][80] = {0};
    clockid_t clockid = CLOCK_MONOTONIC;

    struct timespec start_time, end_time;
    clock_gettime(clockid, &start_time);
    mandelbrot(80, 34, (int*)table, 300000);
    clock_gettime(clockid, &end_time);

    struct timespec time_diff = tdiff(&start_time, &end_time);
    printf("Calculated Mandelbrot set in %lf (s)\n", (double)time_diff.tv_sec + time_diff.tv_nsec*1.0e-9);

    print_image(80, 34, (int*)table);

    return 0;
}
