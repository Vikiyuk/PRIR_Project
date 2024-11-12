#include <stdio.h>
#include <omp.h>
#include <float.h>
#include <math.h>

#define LOWER_BOUND 1.0
#define UPPER_BOUND 1000.0
#define STEP 0.00001  // Zwiększona liczba iteracji

double func(double x) {
    return sin(x) * log(x + 1001) - cos(x / 2);
}

void find_minimum(int num_threads) {
    double global_min = DBL_MAX;
    double global_min_x = LOWER_BOUND;
    double start_time, end_time;
    int num_steps = (int)((UPPER_BOUND - LOWER_BOUND) / STEP);

    start_time = omp_get_wtime();

    // Równoległa pętla for z redukcją i dynamicznym harmonogramem
    #pragma omp parallel for num_threads(num_threads) schedule(dynamic) reduction(min:global_min)
    for (int i = 0; i <= num_steps; i++) {
        double x = LOWER_BOUND + i * STEP;
        double value = func(x);

        if (value < global_min) {
            global_min = value;
            global_min_x = x;
        }
    }

    end_time = omp_get_wtime();
    printf("Threads: %d, Minimum value: %lf at x = %lf, Time: %lf seconds\n", num_threads, global_min, global_min_x, end_time - start_time);

    FILE *file = fopen("results.csv", "a");
    if (file != NULL) {
        fprintf(file, "%d,%lf\n", num_threads, end_time - start_time);
        fclose(file);
    } else {
        printf("Error opening file!\n");
    }
}

int main() {
    FILE *file = fopen("results.csv", "w");
    if (file != NULL) {
        fprintf(file, "Number of Processors,Time (s)\n");
        fclose(file);
    }

    for (int num_threads = 1; num_threads <= omp_get_max_threads(); num_threads++) {
        find_minimum(num_threads);
    }

    return 0;
}
