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

void push_point(std::vector<bfreeman::Point>& vec, double x, double y);

void push_edge(std::vector<bfreeman::Edge>& vec, size_t i, size_t j, bool interior, double distance);

bool is_close(bfreeman::Edge e1, bfreeman::Edge e2);

bool compare_al(const AdjacencyList& test_al, const AdjacencyList& true_al);

void run_test(std::string name, AdjacencyList test_al, AdjacencyList true_al, unsigned short& passed, const bool verbose);

void print_info(std::string label, const Polygon& polygon, const PointPair& se);

void print_test_report(const size_t passed_tests, const size_t total_tests);

#endif // #ifndef __TEST_UTIL_HPP__