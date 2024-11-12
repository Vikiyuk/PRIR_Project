#include <stdio.h>
#include <mpi.h>
#include <float.h>
#include <math.h>

#define LOWER_BOUND 1.0
#define UPPER_BOUND 1000.0
#define STEP 0.00001

double func(double x) {
    return sin(x) * log(x + 1001) - cos(x / 2);
}

int main(int argc, char** argv) {
    int rank, size;
    double local_min = DBL_MAX;
    double global_min;
    double local_min_x, global_min_x;
    double start_time, end_time;

    // Inicjalizacja MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Oblicz zakres przeszukiwania dla każdego procesu
    int num_steps = (int)((UPPER_BOUND - LOWER_BOUND) / STEP);
    int steps_per_proc = num_steps / size;
    double local_lower = LOWER_BOUND + rank * steps_per_proc * STEP;
    double local_upper = local_lower + steps_per_proc * STEP;

    // Mierzenie czasu rozpoczęcia
    if (rank == 0) {
        start_time = MPI_Wtime();
    }

    // Znajdowanie lokalnego minimum
    for (double x = local_lower; x <= local_upper; x += STEP) {
        double value = func(x);
        if (value < local_min) {
            local_min = value;
            local_min_x = x;
        }
    }

    // Redukcja do znalezienia globalnego minimum
    MPI_Reduce(&local_min, &global_min, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);

    // Proces główny (rank 0) zapisuje wynik do pliku CSV
    if (rank == 0) {
        end_time = MPI_Wtime();
        double execution_time = end_time - start_time;
        printf("Processes: %d, Global minimum value: %lf, Time: %lf seconds\n", size, global_min, execution_time);

        // Sprawdzenie, czy plik istnieje
        FILE *file = fopen("mpi_results.csv", "r");
        if (file == NULL) {
            // Jeśli plik nie istnieje, dodaj nagłówek
            file = fopen("mpi_results.csv", "w");
            fprintf(file, "Number of Processes,Time (s)\n");
        } else {
            fclose(file);
            file = fopen("mpi_results.csv", "a");
        }

        // Zapis wyników do pliku CSV
        if (file != NULL) {
            fprintf(file, "%d,%lf\n", size, execution_time);
            fclose(file);
        } else {
            printf("Error opening file!\n");
        }
    }
    // Zakończenie MPI
    MPI_Finalize();
    return 0;
}
