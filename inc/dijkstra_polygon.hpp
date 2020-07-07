#ifndef __DIJKSTRA_POLYGON_HPP__
#define __DIJKSTRA_POLYGON_HPP__

#include <cmath>
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
    double weight;
};

struct Segment {
    Point p1;
    Point p2;
};

bool is_neighbor_idx(size_t i, size_t j, const size_t size) {
    size_t temp = i > j ? i : j;
    j = j < i ? j : i;
    i = temp;
    return i - j == 1 || i - j == size - 1;
}

enum Orientation {
    COLINEAR = 0,
    COUNTERCLOCKWISE = 1,
    CLOCKWISE = 2
};

Orientation orientation(const Point& p, const Point& q, const Point& r) {
	int value = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
	if (value == 0) return COLINEAR;
	return value > 0 ? CLOCKWISE : COUNTERCLOCKWISE;
}

// does not count endpoints as instersecting
bool check_intersect(Segment seg1, Segment seg2) {
    Orientation o1 = orientation(seg1.p1, seg1.p2, seg2.p1);
    Orientation o2 = orientation(seg1.p1, seg1.p2, seg2.p2);
    Orientation o3 = orientation(seg2.p1, seg2.p2, seg1.p1);
    Orientation o4 = orientation(seg2.p1, seg2.p2, seg1.p2);
    return o1 != o2 && o3 != o4;
}

bool is_interior_chord(const std::vector<std::vector<Point>>& polygon, const Segment& seg) {
    for (size_t i = 0; i < polygon.size(); i++) {
        size_t curr_idx = 0;
        do {
            size_t next_idx = (curr_idx + 1) % polygon[i].size();
            Segment seg_other = {polygon[i][curr_idx], polygon[i][next_idx]};
            if (check_intersect(seg, seg_other)) return false;
            curr_idx = next_idx;
        } while (curr_idx != 0);
    }
    return true;
}

double sq(const double d) {
    return d*d;
}

double length(const Segment& seg) {
    return sqrt(
        sq(seg.p1.x - seg.p2.x) +
        sq(seg.p1.y - seg.p2.y)
    );
}

void populate_interior_adjacency(const std::vector<std::vector<Point>>& polygon,
                                 const Point& start, const Point& end, std::vector<std::vector<Edge>> adj_list) {
    for (size_t i = 0; i < polygon.size(); i++) {
        for (size_t j = 0; j < polygon[i].size(); j++) {
            IndexPair idxp = {i, j};
            Point vertex = polygon[idxp.i][idxp.j];
            Segment seg_start = {start, vertex};
            Segment seg_end = {end, vertex};

            if (is_interior_chord(polygon, seg_start)) {
                adj_list[0].push_back((Edge){idxp, length(seg_start)});
            }

            if (is_interior_chord(polygon, seg_end)) {
                adj_list[1].push_back((Edge){idxp, length(seg_end)});
            }
        }
    }
}

void populate_vertex_adjacency(const std::vector<std::vector<Point>>& polygon, const Point& start, const Point& end,
                        std::vector<Edge>& adj_list_row, const IndexPair idxp) {
    Point vertex = polygon[idxp.i][idxp.j];

    Segment to_start = {start, vertex};
    if (is_interior_chord(polygon, to_start)) {
        adj_list_row.push_back((Edge){IndexPair(0, 0, true), length(to_start)});
    }

    Segment to_end = {end, vertex};
    if (is_interior_chord(polygon, to_end)) {
        adj_list_row.push_back((Edge){IndexPair(1, 1, true), length(to_end)});
    }

    for (size_t i = 0; i < polygon.size(); i++) {
        for (size_t j = 0; j < polygon[i].size(); j++) {
            if (i == idxp.i && j == idxp.j) continue;

            IndexPair idxp_other = {i, j};
            Point vertex_other = polygon[idxp_other.i][idxp_other.j];
            Segment seg = {vertex, vertex_other};

            if ((i == idxp.i && is_neighbor_idx(j, idxp.j, polygon[i].size())) || is_interior_chord(polygon, seg)) {
                adj_list_row.push_back((Edge){idxp_other, length(seg)});
            }
        }
    }
}

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
std::vector<Point> dijkstra_path(const std::vector<std::vector<Point>>& polygon, const Point& start, const Point& end) {
    if (is_interior_chord(polygon, (Segment){start, end})) {
        return std::vector<Point>(0);
    }

    size_t vertex_count = 0;
    for (size_t i = 0; i < polygon.size(); i++) {
        vertex_count += polygon[i].size();
    }
    /*
     * [0] = start
     * [1] = end
     * [2]...[polygon[0].size() - 1 + 2] = boundary
     * ... = holes
     */
    std::vector<std::vector<Edge>> adj_list(vertex_count + 2);
    size_t adj_list_idx = 0;

    populate_interior_adjacency(polygon, start, end, adj_list);

    for (size_t i = 0; i < polygon.size(); i++) {
        for (size_t j = 0; j < polygon[i].size(); j++) {
            populate_vertex_adjacency(polygon, start, end, adj_list[adj_list_idx++], (IndexPair){i, j});
        }
    }
}

} // namespace bfreeman

#endif // #ifndef __DIJKSTRA_POLYGON_HPP__