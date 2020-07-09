#include <iostream>
#include <iomanip>
#include "test_util.hpp"
#include "dijkstra_polygon_to_string.hpp"

void push_point(std::vector<bfreeman::Point>& vec, double x, double y) {
    vec.push_back((bfreeman::Point){x, y});
}

void push_edge(std::vector<bfreeman::Edge>& vec, size_t i, size_t j, bool interior, double distance) {
    vec.push_back((bfreeman::Edge){(bfreeman::IndexPair){i, j, interior}, distance});
}

bool is_close(bfreeman::Edge e1, bfreeman::Edge e2) {
    return e1.idxp.i == e2.idxp.i &&
           e1.idxp.j == e2.idxp.j &&
           e1.idxp.interior == e1.idxp.interior &&
           bfreeman::is_close(e1.distance, e2.distance);
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
            std::cout << "sublist size " << i << " has " << true_al[i].size() - test_al[i].size()
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

void run_test(std::string name, AdjacencyList test_al, AdjacencyList true_al, unsigned short& passed, const bool verbose) {
    if(compare_al(test_al, true_al)) {
            std::cout << "PASSED " << name << std::endl;
            passed++;
        } else {
            std::cout << "FAILED " << name << std::endl;
        }
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

void print_test_report(const size_t passed_tests, const size_t total_tests) {
    float percent = 100.0f * passed_tests / total_tests;
    std::cout << passed_tests << " PASSED out of " << total_tests << " (" << std::fixed
              << std::setprecision(1) << percent << "%)" << std::endl;
}