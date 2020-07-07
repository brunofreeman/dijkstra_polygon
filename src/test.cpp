#include <cstdlib>
#include <ctime>
#include <iostream>
#include "dijkstra_polygon.hpp"

int irand(const int min, const int max) {
    return (rand() % (max - min + 1)) + min;
}

double drand(const double min, const double max) {
    return drand48() * (max - min) + min;
}

int main(int argc, char** argv) {
    srand(time(NULL));
    srand48(time(NULL));

    

    return 0;
}