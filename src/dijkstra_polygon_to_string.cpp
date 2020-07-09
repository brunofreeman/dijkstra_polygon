#include "dijkstra_polygon_to_string.hpp"

namespace bfreeman {

void pad(std::string& str, const size_t width) {
    if (width > str.size()) {
        str.insert(0, width - str.size(), ' ');
    }
}

std::string to_string(const Point& point) {
    return "(" + std::to_string(point.x) + ", " +
                 std::to_string(point.y) + ")";
}

std::string to_string(const Segment& segment) {
    return to_string(segment.p1) + "-" + to_string(segment.p2);
}

std::string to_string(const IndexPair& idxp) {
    std::string str;
    if (idxp.interior) {
        str = idxp.i == 0 ? "[s]" : "[e]";
    } else {
        if (idxp.i == 0) {
            str = "[b" + std::to_string(idxp.j) + "]";
        } else {
            str = "[h" + std::to_string(idxp.i - 1) + "." + std::to_string(idxp.j) + "]";
        }
    }
    pad(str, 6);
    return str;
}

std::string to_string(const Edge& edge) {
    return to_string(edge.idxp) + ": " + std::to_string(edge.distance);
}

std::string to_string(const std::vector<std::vector<Point>>& polygon, const std::vector<std::vector<Edge>>& adj_list) {
    auto adj_entry = [](std::string& str, const std::vector<std::vector<Edge>>& adj_list,
                        size_t& adj_idx, size_t i, size_t j) {
        str += "  " + to_string((IndexPair){i, j, adj_idx < 2}) + ": ";
        for (size_t k = 0; k < adj_list[adj_idx].size(); k++) {
            str += to_string(adj_list[adj_idx][k]);
            if (k != adj_list[adj_idx].size() - 1) str += ", ";
        }
        str += "\n";
        adj_idx++;
    };

    std::string str = "{\n";
    size_t adj_idx = 0;

    adj_entry(str, adj_list, adj_idx, 0, 0);
    adj_entry(str, adj_list, adj_idx, 1, 1);

    for (size_t i = 0; i < polygon.size(); i++) {
        for (size_t j = 0; j < polygon[i].size(); j++) {
            adj_entry(str, adj_list, adj_idx, i, j);
        }
    }

    str += "}";
    return str;
}

std::string to_string(const std::vector<std::vector<Point>>& polygon) {
    std::string str = "{\n";

    for (size_t i = 0; i < polygon.size(); i++) {
        for (size_t j = 0; j < polygon[i].size(); j++) {
            str += "  " + to_string((IndexPair){i,j}) + ": " + to_string(polygon[i][j]) + "\n";
        }
    }

    str += "}";
    return str;
}

} // namespace bfreeman