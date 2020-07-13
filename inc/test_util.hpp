#ifndef __TEST_UTIL_HPP__
#define __TEST_UTIL_HPP__

#include <vector>
#include "dijkstra_polygon.hpp"

using Polygon = std::vector<std::vector<bfreeman::Point>>;
using AdjacencyList = std::vector<std::vector<bfreeman::Edge>>;

struct PointPair {
    bfreeman::Point start;
    bfreeman::Point end;
};

/*
 * Compares generated values and values designated as correct.
 * Prints the data for each if -v is passed as a program argument
 * or if the test fails.
 */
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
        const bool verbose
);

/*
 * Prints a fraction and percentage of tests passed.
 */
void print_test_report(const size_t passed_tests, const size_t total_tests);

#endif // #ifndef __TEST_UTIL_HPP__