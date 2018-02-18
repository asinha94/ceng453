#include <stdio.h>
#include <stdlib.h>

/************************************************
 Macros
/************************************************/

#define G 6.67408e-11
#define Squared(x) ((x) * (x))

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
            b->position = (Vector){x,y,z};
            b->velocity = (Vector){vx, vy, vz};
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

void updateVector(Vector * s, Vector *d) {
    s->x = d->x;
    s->y = d->y;
    s->z = d->z;
}

void printPositions(Body * b) {
    printf("Final Position: x: %E y: %E z: %E\n", b->position.x, b->position.y, b->position.z);
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

double NewtonsLaw(double m1, double m2, double radius) {
    if (radius <= 1000) {
        return 0;
    }
    return ((G * m1 * m2) / Squared(radius));
}


Vector getForces(Body * exerted_on, Body * exerted_from) {
    Vector v = {0,0,0};
    double m1 = exerted_on->mass;
    double m2 = exerted_from->mass;

    // Get x component
    double rx = exerted_from->position.x - exerted_on->position.x;
    v.x = NewtonsLaw(m1, m2, rx) * isNegative(rx);

    // y component
    double ry = exerted_from->position.y - exerted_on->position.y;
    v.y = NewtonsLaw(m1, m2, ry) * isNegative(ry);

    // z component
    double rz = exerted_from->position.z - exerted_on->position.z;
    v.z = NewtonsLaw(m1, m2, rz) * isNegative(rz);

    return v;
}

Vector getNewPosition(Body * b, Vector * a, double delta_time) {
    Vector new_position;
    Vector v = b->velocity;
    Vector s = b->position;

    new_position.x = s.x + (delta_time * v.x) + (0.5 * a->x * Squared(delta_time));
    new_position.y = s.y + (delta_time * v.y) + (0.5 * a->y * Squared(delta_time));
    new_position.z = s.z + (delta_time * v.z) + (0.5 * a->z * Squared(delta_time));
    return new_position;
}

Vector findNewVelocity(Vector * old_pos, Vector * new_pos, double delta_time) {
    Vector new_velocity;
    new_velocity.x = (new_pos->x - old_pos->x) / delta_time;
    new_velocity.y = (new_pos->y - old_pos->y) / delta_time;
    new_velocity.z = (new_pos->z - old_pos->z) / delta_time;
    return new_velocity;
}



/************************************************
 Main
/************************************************/

int main(int argc, char **argv) {
    Bodies * bodies = getInitialBodies("examples/nbody_initial_new.txt");

    // ALL Times are in ms
    int totalTime = 50000; // in seconds
    int delta_time = 1; // in  

    for (int i = 0; i < totalTime; i += delta_time) {
        // The new positions/velocities in the struct cant be updated 
        // till all the calculations are done with the old balues.
        // So hold it in the array till all the iterations are complete
        Vector new_positions[bodies->size];
        Vector new_velocity[bodies->size];
        for (int j = 0; j < bodies->size; j++) {

            // get all the Forces to find net acceleration
            Vector net_forces = (Vector) {0,0,0};
            for (int k = 0; k < bodies->size; k++) {
                if (k == j) {
                    continue;
                }
                Vector f = getForces(bodies->array[j], bodies->array[k]);
                net_forces.x += f.x;
                net_forces.y += f.y;
                net_forces.z += f.z;
            }
            Vector acc;
            acc.x = net_forces.x / bodies->array[j]->mass;

            // Get new positions
            new_positions[j] = getNewPosition(bodies->array[j], &acc, (double) (delta_time/10));
            new_velocity[j] = findNewVelocity(&bodies->array[j]->position, &new_positions[j], (double) delta_time);
        }

        for (int m = 0; m < bodies->size; m++) {
            updateVector(&bodies->array[m]->position, &new_positions[m]);
            updateVector(&bodies->array[m]->velocity, &new_velocity[m]);
        }
        // Print here for every iteration
    }

    for (int n = 0; n < bodies->size; n++) {
        printPositions(bodies->array[n]);
    }

    freeBodies(bodies); // pun intended
    return 0;
}