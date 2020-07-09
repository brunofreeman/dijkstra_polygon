#include "dijkstra_polygon.hpp"

// TODO: remove when done debugging
#include <iostream>

#include <string>
#include <cmath>

namespace bfreeman {

const double DBL_EPSILON = 10e-7;

bool is_close(const double a, const double b) {
    return fabs(a - b) < DBL_EPSILON;
}

bool operator==(const Point& p, const Point& q) {
    return is_close(p.x, q.x) && is_close(p.y, q.y);
}

bool is_neighbor_idx(size_t i, size_t j, const size_t size) {
    size_t temp = i > j ? i : j;
    j = j < i ? j : i;
    i = temp;
    return i - j == 1 || i - j == size - 1;
}

Orientation orientation(const Point& p, const Point& q, const Point& r) {
	double value = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
	if (is_close(value, 0.0)) {
        return COLINEAR;
    }
	else {
        return value > 0 ? CLOCKWISE : COUNTERCLOCKWISE;
    }
}

bool share_endpoint(const Segment& seg1, const Segment& seg2) {
    return (
        seg1.p1 == seg2.p1 ||
        seg1.p1 == seg2.p2 ||
        seg1.p2 == seg2.p1 ||
        seg1.p2 == seg2.p2
    );
}

bool on_segment(const Segment& seg, const Point& point) {
    auto max = [](double a, double b) {
        return a > b ? a : b;
    };
    auto min = [](double a, double b) {
        return a < b ? a : b;
    };
	return (point.x <= max(seg.p1.x, seg.p2.x) && point.x >= min(seg.p1.x, seg.p2.x) &&
            point.y <= max(seg.p1.y, seg.p2.y) && point.y >= min(seg.p1.y, seg.p2.y));
}

bool check_intersect(const Segment& seg1, const Segment& seg2) {
    if (share_endpoint(seg1, seg2)) return false;
    Orientation o1 = orientation(seg1.p1, seg1.p2, seg2.p1);
    Orientation o2 = orientation(seg1.p1, seg1.p2, seg2.p2);
    Orientation o3 = orientation(seg2.p1, seg2.p2, seg1.p1);
    Orientation o4 = orientation(seg2.p1, seg2.p2, seg1.p2);
    /* if (seg1.p2 == (Point){0.1, -0.1}) {
        std::cout << "o1: " + std::to_string(o1) << ", "
                  << "o2: " + std::to_string(o2) << ", "
                  << "o3: " + std::to_string(o3) << ", "
                  << "o4: " + std::to_string(o4) << std::endl;
        std::cout << std::to_string(on_segment(seg1, seg2.p1)) << ", "
                  << std::to_string(on_segment(seg1, seg2.p2)) << ", "
                  << std::to_string(on_segment(seg2, seg1.p1)) << ", "
                  << std::to_string(on_segment(seg2, seg1.p2)) << std::endl;
    } */
    return (o1 != o2 && o3 != o4) ||
           (o1 == COLINEAR && on_segment(seg1, seg2.p1)) ||
           (o2 == COLINEAR && on_segment(seg1, seg2.p2)) ||
           (o3 == COLINEAR && on_segment(seg2, seg1.p1)) ||
           (o4 == COLINEAR && on_segment(seg2, seg1.p2));
}

// std::string to_string(const Segment& seg);

// assumes !same_hole_non_neighbor
bool is_interior_chord(const std::vector<std::vector<Point>>& polygon, const Segment& seg) {
    for (size_t i = 0; i < polygon.size(); i++) {
        size_t curr_idx = 0;
        do {
            size_t next_idx = (curr_idx + 1) % polygon[i].size();
            Segment seg_other = {polygon[i][curr_idx], polygon[i][next_idx]};
            if (check_intersect(seg, seg_other)) {
                // std::cout << to_string(seg) << " intersects " << to_string(seg_other) << std::endl;
                return false;
            }
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
                                 const Point& start, const Point& end, std::vector<std::vector<Edge>>& adj_list) {
    Segment start_end = {start, end};
    if (is_interior_chord(polygon, start_end)) {
        adj_list[0].push_back((Edge){IndexPair(1, 1, true), length(start_end)});
        adj_list[1].push_back((Edge){IndexPair(0, 0, true), length(start_end)});
    }

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

            bool same_poly = i == idxp.i;
            bool neighbors = same_poly && is_neighbor_idx(j, idxp.j, polygon[i].size());
            bool same_hole_non_neighbor = same_poly && i > 0 && !is_neighbor_idx(j, idxp.j, polygon[i].size());

            if (!same_hole_non_neighbor && (neighbors || is_interior_chord(polygon, seg))) {
                adj_list_row.push_back((Edge){idxp_other, length(seg)});
            }
        }
    }
}

std::vector<std::vector<Edge>> generate_adjacency_list(const std::vector<std::vector<Point>>& polygon,
                                                       const Point& start, const Point& end) {
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

    populate_interior_adjacency(polygon, start, end, adj_list);

    size_t adj_list_idx = 2;
    for (size_t i = 0; i < polygon.size(); i++) {
        for (size_t j = 0; j < polygon[i].size(); j++) {
            populate_vertex_adjacency(polygon, start, end, adj_list[adj_list_idx++], (IndexPair){i, j});
        }
    }

    return adj_list;
}

std::vector<Point> dijkstra_path(const std::vector<std::vector<Point>>& polygon, const Point& start, const Point& end) {
    // TODO: bring this back at the end
    /* if (is_interior_chord(polygon, (Segment){start, end})) {
        return std::vector<Point>(0);
    } */
    std::vector<std::vector<Edge>> adj_list = generate_adjacency_list(polygon, start, end);
    return std::vector<Point>(0);
}

} // namespace bfreeman