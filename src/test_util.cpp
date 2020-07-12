#include <iostream>
#include <iomanip>
#include <cmath>
#include "test_util.hpp"
#include "dijkstra_polygon_to_string.hpp"

const double DBL_EPSILON = 10e-7;
const unsigned char SEPARATION_LINE_LENGTH = 100;

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

void print_seperation_line() {
    for (size_t i = 0; i < SEPARATION_LINE_LENGTH; i++) {
        std::cout << '-';
    }
    std::cout << std::endl;
}

void push_point(std::vector<bfreeman::Point>& vec, double x, double y) {
    vec.push_back((bfreeman::Point){x, y});
}

void push_edge(std::vector<bfreeman::Edge>& vec, size_t i, size_t j, bool interior, double distance) {
    vec.push_back((bfreeman::Edge){(bfreeman::IndexPair){i, j, interior}, distance});
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

bool compare_al(const AdjacencyList& test_al, const AdjacencyList& true_al) {
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
            std::cout << "sublist " << i << " has " << true_al[i].size() - test_al[i].size()
                      << " too few elements: ";
            return false;
        }
    }
    for (size_t i = 0; i < test_al.size(); i++) {
        for (size_t j = 0; j < test_al[i].size(); j++) {
            if (!is_close(test_al[i][j], true_al[i][j])) {
                std::cout << "edge at (" << i << ", " << j << ") incorrect (test: "
                          << bfreeman::to_string(test_al[i][j]) + ", true: " << bfreeman::to_string(true_al[i][j])
                          << "): ";
                return false;
            }
        }
    }
    return true;
}

void run_test(
    std::string name,
    const Polygon& polygon,
    const AdjacencyList& test_al,
    const AdjacencyList& true_al,
    const double test_path_length,
    const double true_path_length,
    unsigned short& passed_tests,
    const bool verbose) {

    bool passed = compare_al(test_al, true_al) && is_close(test_path_length, true_path_length);
    if (verbose || !passed) {
        print_label(name);
        print_polygon(polygon);
        print_label("Generated adjacency list");
        print_adjacency_list(polygon, test_al);
        print_label("Correct adjacency list");
        print_adjacency_list(polygon, true_al);
        print_label("Generated path length:");
        print_adjacency_list(polygon, true_al);
        print_path_lengths(test_path_length, true_path_length);
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