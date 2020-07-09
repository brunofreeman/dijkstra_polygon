#ifndef __DIJKSTRA_POLYGON_HPP__
#define __DIJKSTRA_POLYGON_HPP__

#include <vector>

namespace bfreeman {

struct Point {
    double x;
    double y;
};

struct IndexPair {
    size_t i;
    size_t j;
    bool interior;
    IndexPair(size_t i, size_t j) : i(i), j(j), interior(false) {}
    IndexPair(size_t i, size_t j, bool interior) : i(i), j(j), interior(interior) {}
};

struct Edge {
    IndexPair idxp;
    double distance;
};

struct Segment {
    Point p1;
    Point p2;
};

enum Orientation {
    COLINEAR = 0,
    COUNTERCLOCKWISE = 1,
    CLOCKWISE = 2
};

bool is_close(const double a, const double b);

bool operator==(const Point& p, const Point& q);

bool is_neighbor_idx(size_t i, size_t j, const size_t size);

Orientation orientation(const Point& p, const Point& q, const Point& r);

bool share_endpoint(const Segment& seg1, const Segment& seg2);

bool on_segment(const Segment& seg, const Point& point);

bool check_intersect(const Segment& seg1, const Segment& seg2);

// assumes !same_hole_non_neighbor
bool is_interior_chord(const std::vector<std::vector<Point>>& polygon, const Segment& seg);

double sq(const double d);

double length(const Segment& seg);

void populate_interior_adjacency(const std::vector<std::vector<Point>>& polygon,
                                 const Point& start, const Point& end, std::vector<std::vector<Edge>>& adj_list);

void populate_vertex_adjacency(const std::vector<std::vector<Point>>& polygon, const Point& start, const Point& end,
                        std::vector<Edge>& adj_list_row, const IndexPair idxp);

std::vector<std::vector<Edge>> generate_adjacency_list(const std::vector<std::vector<Point>>& polygon,
                                                       const Point& start, const Point& end);

/*
 * Assumes that start and end are valid interior points of the polygon
 * (i.e., within its boundray and not within any holes).
 *
 * Assumes that the polygon boundary and holes are defined in a
 * clockwise or counterclockwise winding order.
 *
 * Assumes that the polygon is well defined (i.e., no self intersection
 * of the boundary or holes, all holes are compeltely within the
 * boundary, and no holes overlap)
 *
 * @param polygon a represented by (x, y) coordinates.
 *        The first std::vector defines the boundary.
 *        Subsequent std::vectors are define holes.
 * @param start the starting point
 * @param end the ending point
 * @return an std::vector containing all, if any, intermediate
 *         points needed to reach end from start in the shortest
 *         distance without leaving the polygon boundary or
 *         crossing polygon holes
 */
std::vector<Point> dijkstra_path(const std::vector<std::vector<Point>>& polygon, const Point& start, const Point& end);

} // namespace bfreeman

#endif // #ifndef __DIJKSTRA_POLYGON_HPP__