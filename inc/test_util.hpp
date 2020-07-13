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

void push_point(std::vector<bfreeman::Point>& point_vec, double x, double y);

void push_edge(std::vector<bfreeman::Edge>& edge_vec, size_t i, size_t j, bool interior, double distance);

bool is_close(const bfreeman::Edge& e1, const bfreeman::Edge& e2);

bool compare_adjacency_list(const AdjacencyList& test_al, const AdjacencyList& true_al);

bool compare_path(const std::vector<bfreeman::Point>& test_p, const std::vector<bfreeman::Point>& true_p);

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

void print_test_report(const size_t passed_tests, const size_t total_tests);

#endif // #ifndef __TEST_UTIL_HPP__