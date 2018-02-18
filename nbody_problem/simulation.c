#include <stdio.h>
#include <stdlib.h>

/************************************************
 Macros
/************************************************/

#define G 6.67408e-11
#define NewtonsLaw(G,mi,mj,r) ( ((G) * (mi) * (mj)) / ( (r) * (r) ) )


/************************************************
 Typdefs/Structs
/************************************************/

typedef struct vector {
    double x,y,z;
} Vector;

typedef struct body {
    Vector position;
    Vector velocity;
    double mass;
} Body;

typedef struct bodiesList {
    struct body *array[27];
    int size;
} Bodies;

/************************************************
 Helper Functions
/************************************************/

Vector fillVector(double x, double y, double z) {
    Vector v = {x,y,z};
    return v;
}

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
            b->position = fillVector(x,y,z);
            b->velocity = fillVector(vx, vy, vz);
            b->mass = mass;
            bodies->array[i++] = b;
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

/************************************************
 Math Functions
/************************************************/

double isNegative (double num) {
    if (num < 0) {
        return (double)-1;
    }
    return (double) 1;
}


Vector getAcceleration(Body * exerted_on, Body * exerted_from) {
    Vector v = {0,0,0};
    double m1 = exerted_on->mass;
    double m2 = exerted_from->mass;

    // Get x component
    double rx = exerted_from->position.x - exerted_on->position.x;
    v.x = (NewtonsLaw(G, m1, m2, rx) * isNegative(rx)) / m1;

    // y component
    double ry = exerted_from->position.y - exerted_on->position.y;
    v.y = (NewtonsLaw(G, m1, m2, ry) * isNegative(ry)) / m1;

    // z component
    double rz = exerted_from->position.z - exerted_on->position.z;
    v.z = (NewtonsLaw(G, m1, m2, rz) * isNegative(rz)) / m1;

    return v;
}

/************************************************
 Main
/************************************************/

int main(int argc, char **argv) {
    Bodies * bodies = getInitialBodies("nbody_initial.txt");
    int totalTime = 500000; // in seconds
    int delta = 1; 

    for (int i = 0; i < totalTime; i += delta) {
        
    }

    for (int i = 0; i < bodies->size; i ++) {
        Body * b = bodies->array[i]; 
        printf("%d: %f %f %f\n", i, b->position.x, b->position.y, b->position.z);
    }
    

    freeBodies(bodies);
    return 0;
}