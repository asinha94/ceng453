#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

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

double NewtonsLaw(double m1, double m2, double distance) {
    return ((G * m1 * m2) / Squared(distance));
}


Vector getForces(Body * exerted_on, Body * exerted_from) {
    Vector v = (Vector) {0.0, 0.0, 0.0};
    
    // Get distance in each component
    double rx = exerted_from->position.x - exerted_on->position.x;
    double ry = exerted_from->position.y - exerted_on->position.y;
    double rz = exerted_from->position.z - exerted_on->position.z;

    // If distance is < 1000m then force is zero
    double distance = sqrt(Squared(rx) + Squared(ry) + Squared(rz));
    if (distance <= 1000.0) {
        return v;
    }
    
    double force = NewtonsLaw(exerted_on->mass, exerted_from->mass, distance);

    // Use Newtons Law of Gravitation and figure out direction
    v.x = force * (rx / distance);
    v.y = force * (ry / distance);
    v.z = force * (rz / distance);

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

Vector getNewVelocity(Vector * acceleration, Vector * old_velocity, double delta_time) {
    Vector new_velocity;
    new_velocity.x = old_velocity->x + (acceleration->x * delta_time);
    new_velocity.y = old_velocity->y + (acceleration->y * delta_time);
    new_velocity.z = old_velocity->z + (acceleration->z * delta_time);
    return new_velocity;
}


/************************************************
 Main
/************************************************/

int main(int argc, char **argv) {
    Bodies * bodies = getInitialBodies("nbody_initial.txt");

    // ALL Times are in ms
    int totalTime = 50000000;
    int delta_time = 50;

    for (int i = 0; i < totalTime; i += delta_time) {
        // The new positions/velocities in the struct cant be updated 
        // till all the calculations are done with the old values.
        // So hold it in the array till all the iterations are complete
        Vector new_positions[bodies->size];
        Vector new_velocity[bodies->size];
        for (int j = 0; j < bodies->size; j++) {

            // get all the Forces to find net acceleration
            Vector net_forces = (Vector) {0.0,0.0,0.0};
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
            acc.y = net_forces.y / bodies->array[j]->mass;
            acc.z = net_forces.z / bodies->array[j]->mass;

            // Get new positions
            new_positions[j] = getNewPosition(bodies->array[j], &acc, ((double) delta_time)/1000);
            new_velocity[j] = getNewVelocity(&acc, &bodies->array[j]->velocity, ((double) delta_time)/1000);
        }

        for (int m = 0; m < bodies->size; m++) {
            updateVector(&bodies->array[m]->position, &new_positions[m]);
            updateVector(&bodies->array[m]->velocity, &new_velocity[m]);
            
        }
    }

    for (int n = 0; n < bodies->size; n++) {
        printPositions(bodies->array[n]);
    }

    freeBodies(bodies); // pun intended
    return 0;
}