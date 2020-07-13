#include "dijkstra_polygon.hpp"
#include "dijkstra_polygon_to_string.hpp"

#include <queue>
#include <set>
#include <cmath>

namespace bfreeman {

    const double DBL_EPSILON = 10e-7;
    const IndexPair START_IDXP = {0, 0, true};
    const IndexPair END_IDXP = {1, 1, true};

    enum Orientation {
        COLINEAR = 0,
        COUNTERCLOCKWISE = 1,
        CLOCKWISE = 2
    };

    bool is_close(const double a, const double b) {
        return fabs(a - b) < DBL_EPSILON;
    }

    bool operator==(const Point& p, const Point& q) {
        return is_close(p.x, q.x) && is_close(p.y, q.y);
    }

    void operator-=(Point& p, const Point& q) {
        p.x -= q.x;
        p.y -= q.y;
    }

    double sq(const double d) {
        return d * d;
    }

    double length(const Segment& seg) {
        return sqrt(
                sq(seg.p1.x - seg.p2.x) +
                sq(seg.p1.y - seg.p2.y)
        );
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
        } else {
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

    bool on_segment(const Segment& seg, const Point& p) {
        auto max = [](double a, double b) {
            return a > b ? a : b;
        };
        auto min = [](double a, double b) {
            return a < b ? a : b;
        };
        return (p.x <= max(seg.p1.x, seg.p2.x) && p.x >= min(seg.p1.x, seg.p2.x) &&
                p.y <= max(seg.p1.y, seg.p2.y) && p.y >= min(seg.p1.y, seg.p2.y));
    }

    bool check_intersect(const Segment& seg1, const Segment& seg2) {
        if (share_endpoint(seg1, seg2)) return false;
        Orientation o1 = orientation(seg1.p1, seg1.p2, seg2.p1);
        Orientation o2 = orientation(seg1.p1, seg1.p2, seg2.p2);
        Orientation o3 = orientation(seg2.p1, seg2.p2, seg1.p1);
        Orientation o4 = orientation(seg2.p1, seg2.p2, seg1.p2);
        /* if (seg1.p2 == (Point){4, 2}) {
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

    double get_relative_angle(const Point& origin, Point angle_point) {
        angle_point -= origin;
        if (is_close(angle_point.x, 0)) {
            if (angle_point.y > 0) {
                return M_PI / 2;
            } else {
                return 3 * M_PI / 2;
            }
        }
        if (is_close(angle_point.y, 0)) {
            if (angle_point.x > 0) {
                return 2 * M_PI;
            } else {
                return M_PI;
            }
        }

        if (angle_point.x < 0 && angle_point.y < 0) {
            angle_point.x *= -1;
            angle_point.y *= -1;
            return M_PI + atan(angle_point.y / angle_point.x);
        } else if (angle_point.x < 0) {
            angle_point.x *= -1;
            return M_PI - atan(angle_point.y / angle_point.x);
        } else if (angle_point.y < 0) {
            angle_point.y *= -1;
            return 2 * M_PI - atan(angle_point.y / angle_point.x);
        } else {
            return atan(angle_point.y / angle_point.x);
        }
    }

// assumes a clockwise winding
    Point get_angle_range(const std::vector<std::vector<Point>>& polygon, const IndexPair& idxp) {
        Point point_prev = polygon[idxp.i][(idxp.j == 0 ? polygon[idxp.i].size() : idxp.j) - 1];
        Point point = polygon[idxp.i][idxp.j];
        Point point_next = polygon[idxp.i][idxp.j == polygon[idxp.i].size() - 1 ? 0 : idxp.j + 1];

        double angle_prev = get_relative_angle(point, point_prev);
        double angle_next = get_relative_angle(point, point_next);

        if (idxp.i > 0) {
            double temp = angle_next;
            angle_next = angle_prev;
            angle_prev = temp;
        }
        if (angle_next < angle_prev) {
            return (Point) {angle_next, angle_prev};
        } else {
            return (Point) {angle_next - 2 * M_PI, angle_prev};
        }
    }

    bool pointing_inside(Segment segment, const Point& angle_range) {
        double angle = get_relative_angle(segment.p1, segment.p2);
        if (angle_range.x <= angle && angle <= angle_range.y) return true;
        angle -= 2 * M_PI;
        if (angle_range.x <= angle && angle <= angle_range.y) return true;
        return false;
    }

    bool is_interior_chord_vertex_vertex(
            const std::vector<std::vector<Point>>& polygon,
            const IndexPair& from,
            const IndexPair& to) {

        Segment segment = {polygon[from.i][from.j], polygon[to.i][to.j]};
        Point angle_range = get_angle_range(polygon, from);
        if (!pointing_inside(segment, angle_range)) return false;

        return is_interior_chord_start_or_end(polygon, segment);
    }

    bool is_interior_chord_start_or_end(
            const std::vector<std::vector<Point>>& polygon,
            const Segment& segment) {

        for (size_t i = 0; i < polygon.size(); i++) {
            size_t curr_idx = 0;
            do {
                size_t next_idx = (curr_idx + 1) % polygon[i].size();
                Segment seg_other = {polygon[i][curr_idx], polygon[i][next_idx]};
                if (check_intersect(segment, seg_other)) {
                    // std::cout << to_string(seg) << " intersects " << to_string(seg_other) << std::endl;
                    return false;
                }
                curr_idx = next_idx;
            } while (curr_idx != 0);
        }
        return true;
    }

    void populate_interior_adjacency(
            const std::vector<std::vector<Point>>& polygon,
            const Point& start,
            const Point& end,
            std::vector<std::vector<Edge>>& adj_list) {

        Segment start_end = {start, end};
        if (is_interior_chord_start_or_end(polygon, start_end)) {
            adj_list[0].push_back((Edge) {END_IDXP, length(start_end)});
            adj_list[1].push_back((Edge) {START_IDXP, length(start_end)});
        }

        for (size_t i = 0; i < polygon.size(); i++) {
            for (size_t j = 0; j < polygon[i].size(); j++) {
                IndexPair idxp = {i, j};
                Point vertex = polygon[idxp.i][idxp.j];
                Segment seg_start = {start, vertex};
                Segment seg_end = {end, vertex};

                if (is_interior_chord_start_or_end(polygon, seg_start)) {
                    adj_list[0].push_back((Edge) {idxp, length(seg_start)});
                }

                if (is_interior_chord_start_or_end(polygon, seg_end)) {
                    adj_list[1].push_back((Edge) {idxp, length(seg_end)});
                }
            }
        }
    }

    void populate_vertex_adjacency(
            const std::vector<std::vector<Point>>& polygon,
            const Point& start,
            const Point& end,
            std::vector<Edge>& adj_list_row,
            const IndexPair idxp) {

        Point vertex = polygon[idxp.i][idxp.j];


        Segment to_start = {start, vertex};
        if (is_interior_chord_start_or_end(polygon, to_start)) {
            adj_list_row.push_back((Edge) {START_IDXP, length(to_start)});
        }

        Segment to_end = {end, vertex};
        if (is_interior_chord_start_or_end(polygon, to_end)) {
            adj_list_row.push_back((Edge) {END_IDXP, length(to_end)});
        }

        for (size_t i = 0; i < polygon.size(); i++) {
            for (size_t j = 0; j < polygon[i].size(); j++) {
                if (i == idxp.i && j == idxp.j) continue;

                IndexPair idxp_other = {i, j};
                Point vertex_other = polygon[idxp_other.i][idxp_other.j];
                Segment segment = {vertex, vertex_other};

                bool neighbors = i == idxp.i && is_neighbor_idx(j, idxp.j, polygon[i].size());
                //bool same_hole_non_neighbor = same_poly && i > 0 && !is_neighbor_idx(j, idxp.j, polygon[i].size());

                if (/* !same_hole_non_neighbor && ( */neighbors ||
                                                      is_interior_chord_vertex_vertex(polygon, idxp, idxp_other)) {
                    adj_list_row.push_back((Edge) {idxp_other, length(segment)});
                }
            }
        }
    }

    size_t dijkstra_points(const std::vector<std::vector<Point>>& polygon) {
        size_t dijkstra_points = 2;
        for (size_t i = 0; i < polygon.size(); i++) {
            dijkstra_points += polygon[i].size();
        }
        return dijkstra_points;
    }

    std::vector<std::vector<Edge>> generate_adjacency_list(
            const std::vector<std::vector<Point>>& polygon,
            const Point& start,
            const Point& end) {
        /*
         * [0] = start
         * [1] = end
         * [2]...[polygon[0].size() - 1 + 2] = boundary
         * ... = holes
         */
        std::vector<std::vector<Edge>> adj_list(dijkstra_points(polygon));

        populate_interior_adjacency(polygon, start, end, adj_list);

        size_t adj_list_idx = 2;
        for (size_t i = 0; i < polygon.size(); i++) {
            for (size_t j = 0; j < polygon[i].size(); j++) {
                populate_vertex_adjacency(polygon, start, end, adj_list[adj_list_idx++], (IndexPair) {i, j});
            }
        }

        return adj_list;
    }

    size_t idxp_to_idx(const std::vector<std::vector<Point>>& polygon, const IndexPair& idxp) {
        if (idxp.interior) return idxp.i;
        size_t idx = 0;
        for (size_t i = 0; i < idxp.i; i++) {
            idx += polygon[i].size();
        }
        idx += idxp.j;
        return idx + 2;
    }

    IndexPair idx_to_idxp(const std::vector<std::vector<Point>>& polygon, size_t idx) {
        if (idx == 0) return START_IDXP;
        if (idx == 1) return END_IDXP;
        idx -= 2;
        for (size_t i = 0; i < polygon.size(); i++) {
            for (size_t j = 0; j < polygon[i].size(); j++) {
                if (idx-- == 0) return IndexPair(i, j);
            }
        }
    }

    struct PathDistance {
        IndexPair idxp;
        double path_distance;
    };

    struct ComparePathDistance {
        bool operator()(const PathDistance& d1, const PathDistance& d2) {
            return d1.path_distance > d2.path_distance;
        }
    };

    DijkstraData dijkstra_path(
            const std::vector<std::vector<Point>>& polygon,
            const Point& start,
            const Point& end) {

        // TODO: bring this back at the end
        /* if (is_interior_chord(polygon, (Segment){start, end})) {
            return std::vector<Point>(0);
        } */

        std::vector<std::vector<Edge>> adj_list = generate_adjacency_list(polygon, start, end);

        size_t total_points = dijkstra_points(polygon);
        std::priority_queue<PathDistance, std::vector<PathDistance>, ComparePathDistance> point_queue;
        std::vector<double> distances(total_points);


        point_queue.push((PathDistance) {START_IDXP, 0});
        point_queue.push((PathDistance) {END_IDXP, __DBL_MAX__});
        size_t dist_idx = 0;
        distances[dist_idx++] = 0;
        distances[dist_idx++] = __DBL_MAX__;


        for (size_t i = 0; i < polygon.size(); i++) {
            for (size_t j = 0; j < polygon[i].size(); j++) {
                point_queue.push((PathDistance) {IndexPair(i, j), __DBL_MAX__});
                distances[dist_idx++] = __DBL_MAX__;
            }
        }

        std::set<size_t> visited;
        std::vector<size_t> prev_point_in_shortest_path(total_points);

        // Dijkstra
        while (!point_queue.empty()) {

            PathDistance curr_point = point_queue.top();
            point_queue.pop();
            size_t point_idx = idxp_to_idx(polygon, curr_point.idxp);
            visited.insert(point_idx);

            for (size_t i = 0; i < adj_list[point_idx].size(); i++) {
                size_t adj_idx = idxp_to_idx(polygon, adj_list[point_idx][i].idxp);
                if (visited.find(adj_idx) != visited.end()) continue;
                double distance_between = adj_list[point_idx][i].distance;

                if (distances[adj_idx] > distance_between + distances[point_idx]) {
                    distances[adj_idx] = distance_between + distances[point_idx];
                    prev_point_in_shortest_path[adj_idx] = point_idx;
                    point_queue.push((PathDistance) {adj_list[point_idx][i].idxp, distances[adj_idx]});
                }
            }
        }

        std::vector<Point> dijkstra_path;
        size_t backtrack_idx = 1;
        while (prev_point_in_shortest_path[backtrack_idx] != 0) {
            backtrack_idx = prev_point_in_shortest_path[backtrack_idx];
            IndexPair idxp = idx_to_idxp(polygon, backtrack_idx);
            dijkstra_path.insert(dijkstra_path.begin(), polygon[idxp.i][idxp.j]);
        }

        dijkstra_path.insert(dijkstra_path.begin(), start);
        dijkstra_path.push_back(end);

        return (DijkstraData) {dijkstra_path, distances[1]};
    }

} // namespace bfreeman