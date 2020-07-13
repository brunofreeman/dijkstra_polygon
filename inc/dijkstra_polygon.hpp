#ifndef __DIJKSTRA_POLYGON_HPP__
#define __DIJKSTRA_POLYGON_HPP__

#include <vector>

namespace bfreeman {

    struct Point {
        double x;
        double y;
    };

    struct Segment {
        Point p1;
        Point p2;
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

    bool is_interior_chord_vertex_vertex(
            const std::vector<std::vector<Point>>& polygon,
            const IndexPair& from,
            const IndexPair& to
    );

    bool is_interior_chord_start_or_end(
            const std::vector<std::vector<Point>>& polygon,
            const Segment& segment
    );

    std::vector<std::vector<Edge>> generate_adjacency_list(
            const std::vector<std::vector<Point>>& polygon,
            const Point& start,
            const Point& end
    );

    struct DijkstraData {
        std::vector<Point> path;
        double distance;
    };

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
 *        Subsequent std::vectors define holes.
 * @param start the starting point
 * @param end the ending point
 * @return an std::pair containg
 *         first: an std::vector containing the points,
 *         including start and end, needed to reach end from
 *         start in the shortest distance without leaving
 *         the polygon boundary or crossing polygon holes
 *         second: the length of the path
 */
    DijkstraData dijkstra_path(
            const std::vector<std::vector<Point>>& polygon,
            const Point& start,
            const Point& end
    );

} // namespace bfreeman

#endif // #ifndef __DIJKSTRA_POLYGON_HPP__