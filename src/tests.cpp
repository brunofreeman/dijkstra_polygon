#include <cstdlib>
#include <ctime>
#include <iostream>
#include "dijkstra_polygon.hpp"
#include "dijkstra_polygon_to_string.hpp"

using Polygon = std::vector<std::vector<bfreeman::Point>>;

struct PointPair {
    bfreeman::Point start;
    bfreeman::Point end;
};

int irand(const int min, const int max) {
    return (rand() % (max - min + 1)) + min;
}

double drand(const double min, const double max) {
    return drand48() * (max - min) + min;
}

bfreeman::Point p(double x, double y) {
    return (bfreeman::Point){x, y};
}

PointPair pp(double x1, double y1, double x2, double y2) {
    return (PointPair){p(x1, y1), p(x2, y2)};
}

void push(std::vector<bfreeman::Point>& vec, double x, double y) {
    vec.push_back(p(x, y));
}

void print_info(std::string label, const Polygon& polygon, const PointPair& se) {
    std::cout << label + ":" << std::endl;
    std::cout << bfreeman::to_string(polygon) << std::endl;
    std::cout << bfreeman::to_string(polygon, bfreeman::generate_adjacency_list(polygon, se.start, se.end)) << std::endl;
    for (size_t i = 0; i < 80; i++) {
        std::cout << '-';
    }
    std::cout << std::endl;
}

int main(int argc, char** argv) {
    srand(time(NULL));
    srand48(time(NULL));

    bool do_unit_square_q1 = true;
    bool do_unit_square_all_q = true;

    if (do_unit_square_q1) { // PASS
        // counterclockwise winding
        Polygon unit_square_q1(1);
        push(unit_square_q1[0], 0, 0);
        push(unit_square_q1[0], 1, 0);
        push(unit_square_q1[0], 1, 1);
        push(unit_square_q1[0], 0, 1);
        PointPair unit_square_q1_se = pp(0.25, 0.25, 0.75, 0.75);
        print_info("unit_square_q1", unit_square_q1, unit_square_q1_se);
    }

    if (do_unit_square_all_q) {
        // clockwise winding
        Polygon unit_square_all_q(1);
        push(unit_square_all_q[0], -0.5, -0.5);
        push(unit_square_all_q[0], -0.5,  0.5);
        push(unit_square_all_q[0],  0.5,  0.5);
        push(unit_square_all_q[0],  0.5, -0.5);
        PointPair unit_square_all_q_se = pp(-0.25, -0.25, 0.25, 0.25);
        print_info("unit_square_all_q", unit_square_all_q, unit_square_all_q_se);
    }
    


    return 0;
}