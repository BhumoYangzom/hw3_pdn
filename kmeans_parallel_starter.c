#include <stdio.h>
#include <stdlib.h> // for strtol
#include <string.h>
#include <omp.h>
#include <math.h> // pow

#define MAXCHAR 25

int main (int argc, char *argv[])
{
    if( argc != 8)
    {
        printf("USE LIKE THIS: kmeans_parallel n_points points.csv num_threads n_centroids initial_centroid_values.csv final_centroid_values.csv time.csv\n");
        return EXIT_FAILURE;
    }

    int n_points;
    n_points = strtol(argv[1], NULL, 10);

    double* array_x; // from data x points
    double* array_y; // from data y points

    double* array_centroid_x;
    double* array_centroid_y;

    int* local_array_x;
    int* local_array_y;

    FILE *inputFile1;
    FILE *inputFile2;

    char* p1;
    char* p2;

    inputFile1 = fopen(argv[2], "r");
    if (inputFile1 == NULL){
        printf("Could not open file %s",argv[2]);
    }

    int thread_count;
    thread_count = strtol(argv[3], NULL, 10);

    int n_centroids;
    n_centroids = strtol(argv[4], NULL, 10);

    inputFile2 = fopen(argv[5], "r");
    if (inputFile2 == NULL){
        printf("Could not open file %s",argv[5]);
    }

    FILE *outputFile;
    outputFile = fopen(argv[6], "w");

    FILE *outputFile2;
    outputFile2 = fopen(argv[7], "w");

    char str[MAXCHAR];

    array_x = malloc(n_points*sizeof(double));
    array_y = malloc(n_points*sizeof(double));

    array_centroid_x = malloc(n_centroids*sizeof(double));
    array_centroid_y = malloc(n_centroids*sizeof(double));

    // printf("Before storing points\n");

    // Store values of 2D points
    int k = 0;
    while (fgets(str, MAXCHAR, inputFile1) != NULL)
    {
        sscanf( str, "%lf,%lf", &(array_x[k]), &(array_y[k]) );
        // printf("i: %d, x: %lf, y: %lf\n", k,  array_x[k], array_y[k]);
        k++;
    }
    fclose(inputFile1); 

    // Storing centroid values
    k = 0;

    while (fgets(str, MAXCHAR, inputFile2) != NULL)
    {
        sscanf( str, "%lf,%lf", &(array_centroid_x[k]), &(array_centroid_y[k]) );
        // printf("i: %d, x: %lf, y: %lf\n", k,  array_centroid_x[k], array_centroid_y[k]);
        k++;
    }

    fclose(inputFile2); 

    double start = omp_get_wtime();

    //TO DO algorithm
    //
    double avgMoveDist = 0.0;
    int *assignedCentroid = malloc(n_points * sizeof(int));
    double *newCentroidPosX = malloc(n_centroids * sizeof(double));
    double *newCentroidPosY = malloc(n_centroids * sizeof(double));
    int ii;

    for (ii = 0; ii < n_centroids; ++ii)
    {
        newCentroidPosX[ii] = array_centroid_x[ii];
        newCentroidPosY[ii] = array_centroid_y[ii];
    }

    int count = 1;
    // while (count)
    for (int c = 0; c < count; c++)
    {
        int i, j, distx, disty, indexj;
        double totDist;
        double min;
        for (i = 0; i < n_points; ++i)
        {
            totDist = 0.0;
            min = MAX_DOUBLE;
            for (j = 0; j < n_centroids; ++j)
            {
                distx = array_x[i] - newCentroidPosX[j];
                disty = array_y[i] - newCentroidPosY[j];
                totDist = sqrt(distx * distx + disty * disty);
                printf("Total distance: %lf\n", totDist);
                if (totDist < min)
                {
                    indexj = j;
                    printf("indexj: %d\n", indexj);
                    min = totDist;
                }
            }
            assignedCentroid[i] = indexj;
        }

        for (int x = 0; x < n_points; ++x)
        {
            printf("%d\n", assignedCentroid[x]);
        }

        //2nd for loop for finding the new centroid position

        int internalCount;
        double sumX, sumY;
        for (j = 0; j < n_centroids; ++j)
        {
            internalCount = 0;
            sumX = 0;
            sumY = 0;
            for (i = 0; i < n_points; ++i)
            {
                if (assignedCentroid[i] == j)
                {
                    sumX = sumX + array_x[i];
                    sumY = sumY + array_y[i];
                    ++internalCount;
                }
            }

            newCentroidPosX[j] = sumX / internalCount;
            newCentroidPosY[j] = sumY / internalCount;
        }

        //3rd for loop for updating avgMoveDist
        double mvDistX, mvDistY, moveDist;
        double sumMvDist = 0;
        for (i = 0; i < n_centroids; ++i)
        {
            mvDistX = array_centroid_x[i] - newCentroidPosX[i];
            mvDistY = array_centroid_y[i] - newCentroidPosY[i];
            moveDist = sqrt(mvDistX * mvDistX + mvDistY * mvDistY);
            sumMvDist = sumMvDist + moveDist;
        }
        avgMoveDist = sumMvDist / n_centroids;
        printf("avg Move Dis: %lf\n", avgMoveDist);
        if (avgMoveDist > 1.0)
        {
            for (int i =0; i < n_centroids; i++){
                array_centroid_x[i]= newCentroidPosX[i];
                array_centroid_y[i] = newCentroidPosY[i];
            }
            count++;
        }
        else {
            count = 0;
        }
    }
    double end = omp_get_wtime();
    
    // Time calculation (in seconds)

    double time_passed = end - start;

    fprintf(outputFile2, "%f", time_passed);

    fclose (outputFile);
    fclose (outputFile2);

    free(array_x);
    free(array_y);
    free(array_centroid_x);
    free(array_centroid_y);

    return 0;
}