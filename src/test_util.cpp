#include <iostream>
#include <iomanip>
#include <cmath>
#include "test_util.hpp"
#include "dijkstra_polygon_to_string.hpp"

const double DBL_EPSILON = 10e-7;
const unsigned char SEPARATION_LINE_LENGTH = 100;

bool compare_path(const std::vector<bfreeman::Point>& test_p, const std::vector<bfreeman::Point>& true_p);

void print_path_points(const std::vector<bfreeman::Point>& test_p, const std::vector<bfreeman::Point>& true_p);

void print_label(std::string label) {
    std::cout << label + ":" << std::endl;
}

void print_polygon(const Polygon& polygon) {
    std::cout << bfreeman::to_string(polygon) << std::endl;
}

void print_adjacency_list(const Polygon& polygon, const AdjacencyList& adjacency_list) {
    std::cout << bfreeman::to_string(polygon, adjacency_list) << std::endl;
}

void print_path_lengths(const double test_path_length, const double true_path_length) {
    std::cout << "Generated path length: " << test_path_length << ", actual: " << true_path_length << std::endl;
}

void print_path_points(const std::vector<bfreeman::Point>& test_p, const std::vector<bfreeman::Point>& true_p) {
    std::cout << "Generated path:" << std::endl;
    for (size_t i = 0; i < test_p.size(); i++) {
        std::cout << bfreeman::to_string(test_p[i]);
        if (i != test_p.size() - 1) std::cout << ", ";
        else std::cout << std::endl;
    }
    std::cout << "Actual path:" << std::endl;
    for (size_t i = 0; i < true_p.size(); i++) {
        std::cout << bfreeman::to_string(true_p[i]);
        if (i != true_p.size() - 1) std::cout << ", ";
        else std::cout << std::endl;
    }
}

void print_seperation_line() {
    for (size_t i = 0; i < SEPARATION_LINE_LENGTH; i++) {
        std::cout << '-';
    }
    std::cout << std::endl;
}

void push_point(std::vector<bfreeman::Point>& point_vec, double x, double y) {
    point_vec.push_back((bfreeman::Point) {x, y});
}

void push_edge(std::vector<bfreeman::Edge>& edge_vec, size_t i, size_t j, bool interior, double distance) {
    edge_vec.push_back((bfreeman::Edge) {(bfreeman::IndexPair) {i, j, interior}, distance});
}

bool is_close(const double a, const double b) {
    return fabs(a - b) < DBL_EPSILON;
}

bool is_close(const bfreeman::Edge& e1, const bfreeman::Edge& e2) {
    return e1.idxp.i == e2.idxp.i &&
           e1.idxp.j == e2.idxp.j &&
           e1.idxp.interior == e1.idxp.interior &&
           is_close(e1.distance, e2.distance);
}

bool compare_adjacency_list(const AdjacencyList& test_al, const AdjacencyList& true_al) {
    if (test_al.size() > true_al.size()) {
        std::cout << "adjacency list has " << test_al.size() - true_al.size()
                  << " too many elements" << std::endl;
        return false;
    }
    if (test_al.size() < true_al.size()) {
        std::cout << "adjacency list has " << true_al.size() - test_al.size()
                  << " too few elements" << std::endl;
        return false;
    }
    for (size_t i = 0; i < test_al.size(); i++) {
        if (test_al[i].size() > true_al[i].size()) {
            std::cout << "adjacency sublist " << i << " has " << test_al[i].size() - true_al[i].size()
                      << " too many elements" << std::endl;
            return false;
        }
        if (test_al[i].size() < true_al[i].size()) {
            std::cout << "adjacency sublist " << i << " has " << true_al[i].size() - test_al[i].size()
                      << " too few elements" << std::endl;
            return false;
        }
    }
    for (size_t i = 0; i < test_al.size(); i++) {
        for (size_t j = 0; j < test_al[i].size(); j++) {
            if (!is_close(test_al[i][j], true_al[i][j])) {
                std::cout << "adjacency list edge at (" << i << ", " << j << ") incorrect (test: "
                          << bfreeman::to_string(test_al[i][j]) + ", true: " << bfreeman::to_string(true_al[i][j])
                          << ")" << std::endl;
                return false;
            }
        }
    }
    return true;
}

bool compare_path(const std::vector<bfreeman::Point>& test_p, const std::vector<bfreeman::Point>& true_p) {
    if (test_p.size() > true_p.size()) {
        std::cout << "path has " << test_p.size() - true_p.size()
                  << " too many elements" << std::endl;
        return false;
    }
    if (test_p.size() < true_p.size()) {
        std::cout << "path has " << true_p.size() - test_p.size()
                  << " too few elements" << std::endl;
        return false;
    }
    for (size_t i = 0; i < test_p.size(); i++) {
        if (!is_close(test_p[i].x, true_p[i].x) || !is_close(test_p[i].y, true_p[i].y)) {
            std::cout << "path point at index " << i << " incorrect: test: "
                      << bfreeman::to_string(test_p[i]) + ", true: " << bfreeman::to_string(true_p[i])
                      << std::endl;
            return false;
        }
    }
    return true;
}

void run_test(
        std::string name,
        const Polygon& polygon,
        const AdjacencyList& test_adjacency_list,
        const AdjacencyList& true_adjacency_list,
        const double test_path_length,
        const double true_path_length,
        const std::vector<bfreeman::Point>& test_path_points,
        const std::vector<bfreeman::Point>& true_path_points,
        unsigned short& passed_tests,
        const bool verbose) {

    bool passed = compare_adjacency_list(test_adjacency_list, true_adjacency_list)
                  && is_close(test_path_length, true_path_length)
                  && compare_path(test_path_points, true_path_points);
    if (verbose || !passed) {
        print_label(name);
        print_polygon(polygon);
        print_label("Generated adjacency list");
        print_adjacency_list(polygon, test_adjacency_list);
        print_label("Correct adjacency list");
        print_adjacency_list(polygon, true_adjacency_list);
        print_label("Generated path length:");
        print_adjacency_list(polygon, true_adjacency_list);
        print_path_lengths(test_path_length, true_path_length);
        print_path_points(test_path_points, true_path_points);
        print_seperation_line();
    }
    if (passed) {
        std::cout << "PASSED " << name << std::endl;
        passed_tests++;
    } else {
        std::cout << "FAILED " << name << std::endl;
    }
}

void print_test_report(const size_t passed_tests, const size_t total_tests) {
    float percent = 100.0f * passed_tests / total_tests;
    std::cout << "PASSED " << passed_tests << " out of " << total_tests << " tests ("
              << std::fixed << std::setprecision(1) << percent << "%)" << std::endl;
}