#ifndef __DIJKSTRA_POLYGON_HPP__
#define __DIJKSTRA_POLYGON_HPP__

#include <vector>

namespace bfreeman {

const size_t START_IDX = 0;
const size_t END_IDX = 1;

struct Point {
    double x;
    double y;
};

struct Segment {
    Point p1;
    Point p2;
};

/*
 * Represents a vertex of the polygon if interior is false
 * or the start/end point if interior is true.
 */
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

struct DijkstraData {
    std::vector<Point> path;
    double distance;
};

/*
 * Determines if a segment from one polygon (or hole)
 * vertex to another is completely inside of the polygon.
 */
bool is_interior_chord_vertex_vertex(
        const std::vector<std::vector<Point>>& polygon,
        const IndexPair& from,
        const IndexPair& to
);

/*
 * Determines if a segment that includes start or end
 * as one of its endpoints is completely inside of the polygon.
 */
bool is_interior_chord_start_or_end(
        const std::vector<std::vector<Point>>& polygon,
        const Segment& segment
);

/*
 * Generates the adjacency list of the graph representing
 * the polygon and holes. The nodes of the graph are the
 * vertices of the polygon and holes, the start point, and
 * the end point. The edges are segments between nodes that
 * are completely inside the polygon (and not in any holes).
 */
std::vector<std::vector<Edge>> generate_adjacency_list(
        const std::vector<std::vector<Point>>& polygon,
        const Point& start,
        const Point& end
);

/*
 * Assumes that start and end are valid interior points of the polygon
 * (i.e., within its boundary and not within any holes).
 *
 * Assumes that the polygon boundary and holes are defined in a
 * counterclockwise winding order.
 *
 * Assumes that the polygon is well defined (i.e., no self intersection
 * of the boundary or holes, all holes are completely within the
 * boundary, and no holes overlap)
 *
 * @param polygon a represented by (x, y) coordinates.
 *        The first std::vector defines the boundary.
 *        Subsequent std::vectors define holes.
 * @param start the starting point
 * @param end the ending point
 * @return a DijkstraData struct with 1) an std::vector
 *         containing the points, including start and end,
 *         needed to reach end from start in the shortest
 *         distance without leaving the polygon boundary
 *         or crossing polygon holes, and 2) the length
 *         of the path
 */
DijkstraData dijkstra_path(
        const std::vector<std::vector<Point>>& polygon,
        const Point& start,
        const Point& end
);

} // namespace bfreeman

#endif // #ifndef __DIJKSTRA_POLYGON_HPP__