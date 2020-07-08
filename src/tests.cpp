#include <iostream>
#include <iomanip>
#include "dijkstra_polygon.hpp"
#include "dijkstra_polygon_to_string.hpp"

using Polygon = std::vector<std::vector<bfreeman::Point>>;
using AdjacencyList = std::vector<std::vector<bfreeman::Edge>>;

struct PointPair {
    bfreeman::Point start;
    bfreeman::Point end;
};

bfreeman::Point p(double x, double y) {
    return (bfreeman::Point){x, y};
}

PointPair pp(double x1, double y1, double x2, double y2) {
    return (PointPair){p(x1, y1), p(x2, y2)};
}

bfreeman::Edge e(size_t i, size_t j, bool interior, double dist) {
    return (bfreeman::Edge){(bfreeman::IndexPair){i, j ,interior}, dist};
}

void push(std::vector<bfreeman::Point>& vec, double x, double y) {
    vec.push_back(p(x, y));
}

void push(std::vector<bfreeman::Edge>& vec, size_t i, size_t j, bool interior, double dist) {
    vec.push_back(e(i, j, interior, dist));
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

bool is_close(bfreeman::Edge e1, bfreeman::Edge e2) {
    return e1.idxp.i == e2.idxp.i &&
           e1.idxp.j == e2.idxp.j &&
           e1.idxp.interior == e1.idxp.interior &&
           bfreeman::is_close(e1.distance, e2.distance);
}

bool compare_al(AdjacencyList test_al, AdjacencyList true_al) {
    if (test_al.size() > true_al.size()) {
        std::cout << "list has " << test_al.size() - true_al.size()
                  << " too many elements" << std::endl;
        return false;
    }
    if (test_al.size() < true_al.size()) {
        std::cout << "list has " << true_al.size() - test_al.size()
                  << " too few elements" << std::endl;
        return false;
    }
    for (size_t i = 0; i < test_al.size(); i++) {
        if (test_al[i].size() > true_al[i].size()) {
            std::cout << "sublist (" << i << ") has " << test_al[i].size() - true_al[i].size()
                      << " too many elements" << std::endl;
            return false;
        }
        if (test_al[i].size() < true_al[i].size()) {
            std::cout << "sublist size (" << i << ") has " << true_al[i].size() - test_al[i].size()
                      << " too few elements" << std::endl;
            return false;
        }
    }
    for (size_t i = 0; i < test_al.size(); i++) {
        for (size_t j = 0; j < test_al[i].size(); j++) {
            if (!is_close(test_al[i][j], true_al[i][j])) {
                std::cout << "edge at (" << i << ", " << j << ") not correct" << std::endl;
                return false;
            }
        }
    }
    return true;
}

void run_test(std::string name, AdjacencyList test_al, AdjacencyList true_al, unsigned short& passed, unsigned short& total) {
    if(compare_al(test_al, true_al)) {
            std::cout << "PASSED " << name << std::endl;
            passed++;
        } else {
            std::cout << "FAILED " << name << std::endl;
        }
        total++;
}

int main(int argc, char** argv) {
    bool verbose = argc > 1 && std::strcmp(argv[1], "-v") == 0;

    unsigned short passed = 0;
    unsigned short total = 0;

    /*
     * Each adjacency list should list start, end,
     * then the border points in winding order,
     * then each hole in order and its points in
     * winding order.
     * Within each sublist, the same ordering holds.
     */

    /*
     * @name unit_square_q1
     * @shape unit square
     * @start off the bottom left corner
     * @end off the top right corner
     * @location bottom left corner on origin
     * @winding counterclockwise
     */
    {
        std::string name = "unit_sq_q1";
        Polygon poly(1);

        push(poly[0], 0, 0);
        push(poly[0], 1, 0);
        push(poly[0], 1, 1);
        push(poly[0], 0, 1);

        PointPair se = pp(0.25, 0.25, 0.75, 0.75);
        if (verbose) print_info(name, poly, se);
        
        AdjacencyList test_al = bfreeman::generate_adjacency_list(poly, se.start, se.end);
        AdjacencyList true_al(6);

        push(true_al[0], 1, 1,  true, 0.707107);
        push(true_al[0], 0, 0, false, 0.353553);
        push(true_al[0], 0, 1, false, 0.790569);
        push(true_al[0], 0, 2, false, 1.060660);
        push(true_al[0], 0, 3, false, 0.790569);

        push(true_al[1], 0, 0,  true, 0.707107);
        push(true_al[1], 0, 0, false, 1.060660);
        push(true_al[1], 0, 1, false, 0.790569);
        push(true_al[1], 0, 2, false, 0.353553);
        push(true_al[1], 0, 3, false, 0.790569);

        push(true_al[2], 0, 0,  true, 0.353553);
        push(true_al[2], 1, 1,  true, 1.060660);
        push(true_al[2], 0, 1, false, 1.000000);
        push(true_al[2], 0, 2, false, 1.414214);
        push(true_al[2], 0, 3, false, 1.000000);

        push(true_al[3], 0, 0,  true, 0.790569);
        push(true_al[3], 1, 1,  true, 0.790569);
        push(true_al[3], 0, 0, false, 1.000000);
        push(true_al[3], 0, 2, false, 1.000000);
        push(true_al[3], 0, 3, false, 1.414214);

        push(true_al[4], 0, 0,  true, 1.060660);
        push(true_al[4], 1, 1,  true, 0.353553);
        push(true_al[4], 0, 0, false, 1.414214);
        push(true_al[4], 0, 1, false, 1.000000);
        push(true_al[4], 0, 3, false, 1.000000);

        push(true_al[5], 0, 0,  true, 0.790569);
        push(true_al[5], 1, 1,  true, 0.790569);
        push(true_al[5], 0, 0, false, 1.000000);
        push(true_al[5], 0, 1, false, 1.414214);
        push(true_al[5], 0, 2, false, 1.000000);

        run_test(name, test_al, true_al, passed, total);
    }
    /*
     * @name unit_square_all_q
     * @shape unit square
     * @start off the bottom left corner
     * @end off the top right corner
     * @location centered on origin
     * @winding clockwise
     */
    {
        std::string name = "unit_sq_all_q";
        Polygon poly(1);

        push(poly[0], -0.5, -0.5);
        push(poly[0], -0.5,  0.5);
        push(poly[0],  0.5,  0.5);
        push(poly[0],  0.5, -0.5);

        PointPair se = pp(-0.25, -0.25, 0.25, 0.25);
        if (verbose) print_info(name, poly, se);

        AdjacencyList test_al = bfreeman::generate_adjacency_list(poly, se.start, se.end);
        AdjacencyList true_al(6);

        push(true_al[0], 1, 1,  true, 0.707107);
        push(true_al[0], 0, 0, false, 0.353553);
        push(true_al[0], 0, 1, false, 0.790569);
        push(true_al[0], 0, 2, false, 1.060660);
        push(true_al[0], 0, 3, false, 0.790569);

        push(true_al[1], 0, 0,  true, 0.707107);
        push(true_al[1], 0, 0, false, 1.060660);
        push(true_al[1], 0, 1, false, 0.790569);
        push(true_al[1], 0, 2, false, 0.353553);
        push(true_al[1], 0, 3, false, 0.790569);

        push(true_al[2], 0, 0,  true, 0.353553);
        push(true_al[2], 1, 1,  true, 1.060660);
        push(true_al[2], 0, 1, false, 1.000000);
        push(true_al[2], 0, 2, false, 1.414214);
        push(true_al[2], 0, 3, false, 1.000000);

        push(true_al[3], 0, 0,  true, 0.790569);
        push(true_al[3], 1, 1,  true, 0.790569);
        push(true_al[3], 0, 0, false, 1.000000);
        push(true_al[3], 0, 2, false, 1.000000);
        push(true_al[3], 0, 3, false, 1.414214);

        push(true_al[4], 0, 0,  true, 1.060660);
        push(true_al[4], 1, 1,  true, 0.353553);
        push(true_al[4], 0, 0, false, 1.414214);
        push(true_al[4], 0, 1, false, 1.000000);
        push(true_al[4], 0, 3, false, 1.000000);

        push(true_al[5], 0, 0,  true, 0.790569);
        push(true_al[5], 1, 1,  true, 0.790569);
        push(true_al[5], 0, 0, false, 1.000000);
        push(true_al[5], 0, 1, false, 1.414214);
        push(true_al[5], 0, 2, false, 1.000000);

        run_test(name, test_al, true_al, passed, total);
    }
    /*
     * @name sq_in_sq
     * @shape unit square with 0.2-side square hole centered
     * @start off the bottom left corner
     * @end off the top right corner
     * @location centered on origin
     * @winding clockwise (boundary), counterclockwise (hole)
     */
    {
        std::string name = "sq_in_sq";
        Polygon poly(2);

        push(poly[0], -0.5, -0.5);
        push(poly[0], -0.5,  0.5);
        push(poly[0],  0.5,  0.5);
        push(poly[0],  0.5, -0.5);
        push(poly[1], -0.1, -0.1);
        push(poly[1],  0.1, -0.1);
        push(poly[1],  0.1,  0.1);
        push(poly[1], -0.1,  0.1);

        PointPair se = pp(-0.4, -0.4, 0.4, 0.4);
        if (verbose) print_info(name, poly, se);

        AdjacencyList test_al = bfreeman::generate_adjacency_list(poly, se.start, se.end);
        AdjacencyList true_al(10);

        push(true_al[0], 0, 0, false, 0.141421);
        push(true_al[0], 0, 1, false, 0.905539);
        push(true_al[0], 0, 3, false, 0.905539);
        push(true_al[0], 1, 0, false, 0.424264);
        push(true_al[0], 1, 1, false, 0.583095);
        push(true_al[0], 1, 3, false, 0.583095);

        push(true_al[1], 0, 1, false, 0.905539);
        push(true_al[1], 0, 2, false, 0.141421);
        push(true_al[1], 0, 3, false, 0.905539);
        push(true_al[1], 1, 1, false, 0.583095);
        push(true_al[1], 1, 2, false, 0.424264);
        push(true_al[1], 1, 3, false, 0.583095);

        push(true_al[2], 0, 0,  true, 0.141421);
        push(true_al[2], 0, 1, false, 1.000000);
        push(true_al[2], 0, 3, false, 1.000000);
        push(true_al[2], 1, 0, false, 0.565685);
        push(true_al[2], 1, 1, false, 0.721110);
        push(true_al[2], 1, 3, false, 0.721110);

        push(true_al[3], 0, 0,  true, 0.905539);
        push(true_al[3], 1, 1,  true, 0.905539);
        push(true_al[3], 0, 0, false, 1.000000);
        push(true_al[3], 0, 2, false, 1.000000);
        push(true_al[3], 1, 0, false, 0.721110);
        push(true_al[3], 1, 2, false, 0.721110);
        push(true_al[3], 1, 3, false, 0.565685);

        push(true_al[4], 1, 1,  true, 0.141421);
        push(true_al[4], 0, 1, false, 1.000000);
        push(true_al[4], 0, 3, false, 1.000000);
        push(true_al[4], 1, 1, false, 0.721110);
        push(true_al[4], 1, 2, false, 0.565685);
        push(true_al[4], 1, 3, false, 0.721110);

        push(true_al[5], 0, 0,  true, 0.905539);
        push(true_al[5], 1, 1,  true, 0.905539);
        push(true_al[5], 0, 0, false, 1.000000);
        push(true_al[5], 0, 2, false, 1.000000);
        push(true_al[5], 1, 0, false, 0.721110);
        push(true_al[5], 1, 1, false, 0.565685);
        push(true_al[5], 1, 2, false, 0.721110);

        push(true_al[6], 0, 0,  true, 0.424264);
        push(true_al[6], 0, 0, false, 0.565685);
        push(true_al[6], 0, 1, false, 0.721110);
        push(true_al[6], 0, 3, false, 0.721110);
        push(true_al[6], 1, 1, false, 0.200000);
        push(true_al[6], 1, 3, false, 0.200000);

        push(true_al[7], 0, 0,  true, 0.583095);
        push(true_al[7], 1, 1,  true, 0.583095);
        push(true_al[7], 0, 0, false, 0.721110);
        push(true_al[7], 0, 2, false, 0.721110);
        push(true_al[7], 0, 3, false, 0.565685);
        push(true_al[7], 1, 0, false, 0.200000);
        push(true_al[7], 1, 2, false, 0.200000);

        push(true_al[8], 1, 1,  true, 0.424264);
        push(true_al[8], 0, 1, false, 0.721110);
        push(true_al[8], 0, 2, false, 0.565685);
        push(true_al[8], 0, 3, false, 0.721110);
        push(true_al[8], 1, 1, false, 0.200000);
        push(true_al[8], 1, 3, false, 0.200000);

        push(true_al[9], 0, 0,  true, 0.583095);
        push(true_al[9], 1, 1,  true, 0.583095);
        push(true_al[9], 0, 0, false, 0.721110);
        push(true_al[9], 0, 1, false, 0.565685);
        push(true_al[9], 0, 2, false, 0.721110);
        push(true_al[9], 1, 0, false, 0.200000);
        push(true_al[9], 1, 2, false, 0.200000);

        run_test(name, test_al, true_al, passed, total);
    }
    /*
     * @name mountain
     * @shape two-peaked mountain with consistent slope
     *        and evenly spaced peaks
     * @start within the left peak
     * @end within the right peak
     * @location bottom left corner on origin
     * @winding counterclockwise
     */
    // counterclockwise winding
    {
        std::string name = "mountain";
        Polygon poly(1);

        push(poly[0], 0, 0);
        push(poly[0], 6, 0);
        push(poly[0], 4, 3);
        push(poly[0], 3, 1.5);
        push(poly[0], 2, 3);

        PointPair se = pp(2, 2, 4, 2);
        if (verbose) print_info(name, poly, se);

        AdjacencyList test_al = bfreeman::generate_adjacency_list(poly, se.start, se.end);
        AdjacencyList true_al(7);

        push(true_al[0], 0, 0, false, 2.828427);
        push(true_al[0], 0, 3, false, 1.118033);
        push(true_al[0], 0, 4, false, 1.000000);

        push(true_al[1], 0, 1, false, 2.828427);
        push(true_al[1], 0, 2, false, 1.000000);
        push(true_al[1], 0, 3, false, 1.118033);

        push(true_al[2], 0, 0,  true, 2.828427);
        push(true_al[2], 0, 1, false, 6.000000);
        push(true_al[2], 0, 3, false, 3.354101);
        push(true_al[2], 0, 4, false, 3.605551);

        push(true_al[3], 1, 1,  true, 2.828427);
        push(true_al[3], 0, 0, false, 6.000000);
        push(true_al[3], 0, 2, false, 3.605551);
        push(true_al[3], 0, 3, false, 3.354101);

        push(true_al[4], 1, 1,  true, 1.000000);
        push(true_al[4], 0, 1, false, 3.605551);
        push(true_al[4], 0, 3, false, 1.802775);

        push(true_al[5], 0, 0,  true, 1.118033);
        push(true_al[5], 1, 1,  true, 1.118033);
        push(true_al[5], 0, 0, false, 3.354101);
        push(true_al[5], 0, 1, false, 3.354101);
        push(true_al[5], 0, 2, false, 1.802775);
        push(true_al[5], 0, 4, false, 1.802775);

        push(true_al[6], 0, 0,  true, 1.000000);
        push(true_al[6], 0, 0, false, 3.605551);
        push(true_al[6], 0, 3, false, 1.802775);

        run_test(name, test_al, true_al, passed, total);
    }

    float percent = 100.0f * passed / total;
    std::cout << passed << " PASSED out of " << total << " (" << std::fixed
              << std::setprecision(1) << percent << "%)" << std::endl;

    return 0;
}