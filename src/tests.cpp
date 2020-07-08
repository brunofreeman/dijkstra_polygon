#include <iostream>
#include <iomanip>
#include <fstream>
#include "dijkstra_polygon.hpp"
#include "dijkstra_polygon_to_string.hpp"

using Polygon = std::vector<std::vector<bfreeman::Point>>;
using AdjacencyList = std::vector<std::vector<bfreeman::Edge>>;
const std::string TEST_FOLDER = "test_data";
const std::string ADJACENCY_LIST_EXTENSION = "_al";

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

AdjacencyList read_al(std::string name, size_t points) {
    AdjacencyList true_al(points);

    std::fstream data;
    data.open("./" + TEST_FOLDER + "/" + name + ADJACENCY_LIST_EXTENSION, std::ios::in);

    std::string line;
    while (getline(data, line)) {
        line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());

        size_t adj_idx, i, j;
        bool interior;
        double distance;

        size_t str_idx = 0;
        size_t data_idx = 0;
        std::string datum;
        while ((str_idx = line.find(',')) != std::string::npos) {
            datum = line.substr(0, str_idx);
            line.erase(0, str_idx + 1);
            switch (data_idx++) {
                case 0:
                    adj_idx = std::stoi(datum);
                    break;
                case 1:
                    i = std::stoi(datum);
                    break;
                case 2:
                    j = std::stoi(datum);
                    break;
                case 3:
                    interior = std::strcmp(&datum[0], "true") == 0;
                    break;
            }
        }
        distance = std::stod(line);
        push(true_al[adj_idx], i, j, interior, distance);
    }

    data.close();
    return true_al;
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
                  << " too many elements: ";
        return false;
    }
    if (test_al.size() < true_al.size()) {
        std::cout << "list has " << true_al.size() - test_al.size()
                  << " too few elements: ";
        return false;
    }
    for (size_t i = 0; i < test_al.size(); i++) {
        if (test_al[i].size() > true_al[i].size()) {
            std::cout << "sublist " << i << " has " << test_al[i].size() - true_al[i].size()
                      << " too many elements: ";
            return false;
        }
        if (test_al[i].size() < true_al[i].size()) {
            std::cout << "sublist size " << i << " has " << true_al[i].size() - test_al[i].size()
                      << " too few elements: ";
            return false;
        }
    }
    for (size_t i = 0; i < test_al.size(); i++) {
        for (size_t j = 0; j < test_al[i].size(); j++) {
            if (!is_close(test_al[i][j], true_al[i][j])) {
                std::cout << "edge at (" << i << ", " << j << ") not correct: ";
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
        AdjacencyList true_al = read_al(name, 6);

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
        AdjacencyList true_al = read_al(name, 6);

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
        AdjacencyList true_al = read_al(name, 10);

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
        AdjacencyList true_al = read_al(name, 7);

        run_test(name, test_al, true_al, passed, total);
    }

    float percent = 100.0f * passed / total;
    std::cout << passed << " PASSED out of " << total << " (" << std::fixed
              << std::setprecision(1) << percent << "%)" << std::endl;

    return 0;
}