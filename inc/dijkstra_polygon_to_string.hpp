#ifndef __DIJKSTRA_POLYGON_TO_STRING_HPP__
#define __DIJKSTRA_POLYGON_TO_STRING_HPP__

#include <string>
#include "dijkstra_polygon.hpp"

namespace bfreeman {

void pad(std::string& str, const size_t width);

std::string to_string(const IndexPair& idxp);

std::string to_string(const Edge& edge);

std::string to_string(const std::vector<std::vector<Point>>& polygon, const std::vector<std::vector<Edge>>& adj_list);

std::string to_string(const Point& point);

std::string to_string(const Segment& seg);

std::string to_string(const std::vector<std::vector<Point>>& polygon);

} // namespace bfreeman

#endif // #ifndef __DIJKSTRA_POLYGON_TO_STRING_HPP__