#include <stdio.h>
#include <stdlib.h>

typedef struct body {
    double x,y,z;
    double mass;
    double v_x, v_y, v_z;
} Body;


typedef struct bodiesList {
    struct body *array[27];
    int size;
} Bodies;

void freeBodies(Bodies * bodies) {
    for (int i = 0; i < bodies->size; i++) {
        Body * b = bodies->array[i];
        free(b);
    }
    free(bodies);
}


Bodies * getInitialBodies(const char * filename) {
    FILE *fp = fopen(filename, "r");
    // get rid of first line
    char buff[255];
    fgets(buff, 255, fp);

    // The real data
    Bodies * bodies = malloc(sizeof(Bodies));
    
    double x,y,z;
    double mass;
    double vx,vy,vz;
    int i = 0;
     while (1) {
        int ret = fscanf(fp, "%lf %lf %lf %lf %lf %lf %lf ", &x, &y, &z, &mass, &vx, &vy, &vz);
        if(ret == 7) {
            Body * b = malloc(sizeof(Body));
            b->x = x;
            b->y = y;
            b->z = z;
            b->mass = mass;
            b->v_x = vx;
            b->v_y = vy;
            b->v_z = vz;
            bodies->array[i] = b;
            i++;
        } else if(ret == EOF) {
            bodies->size = i;
            break;
        } else {
            printf("No match.\n");
        }
    }

    fclose(fp);
    return bodies;
}

int main(int argc, char **argv) {
    
    Bodies * bodies = getInitialBodies("nbody_initial.txt");

    for (int i = 0; i < bodies->size; i ++) {
        Body * b = bodies->array[i]; 
        printf("%d: %f %f %f\n", i, b->x, b->y, b->z);
    }
    

    freeBodies(bodies);
    return 0;
}