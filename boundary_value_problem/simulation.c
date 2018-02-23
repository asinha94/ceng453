#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

    if (argc != 4) {
        printf("Need 3 CLI arguments. Got %d\n\tUSAGE: ./simulation [time (s)] [segment length] [r]\n", argc-1);
        return 1;
    }

    int m = atoi(argv[1]); // total time in seconds
    int n = atoi(argv[2]); // length subdivisions
    double r = atof(argv[3]);

    if (m == 0 || n == 0 || r == 0.0) {
        printf("Can't have 0 as a value for anything!\n\n");
        return 1;
    }

    double temp[m][n];

    for (int i=1; i<n-1; i++) {
        temp[0][i] = 20.0;
    }

    temp[0][0] = 0.0;
    temp[0][n-1] = 100.0;

    for (int time = 1; time < m; time++) {
        temp[time][0] = 0.0;
        temp[time][n-1] = 100.0;

        for (int segment = 1; segment < n-1; segment++) {
            temp[time][segment] = r * temp[time-1][segment-1] + (1-2*r)*temp[time-1][segment] + r*temp[time-1][segment+1];
        }
    }

    printf("Printing All %dth segment values", n/4);
    for (int i = 0; i < m; i++) {
        printf("%.2f, ", temp[i][n/4]);
    }
    printf("\n");

    printf("Printing All %dth segment values", n/2);
    for (int i = 0; i < m; i++) {
        printf("%.2f, ", temp[i][n/2]);
    }
    printf("\n");

    printf("Printing All %dth segment values\n", 3*n/4);
    for (int i = 0; i < m; i++) {
        printf("%f, ", temp[i][3*n/4]);
    }
    printf("\n");

}