#include <fstream>
#include "test_data_reader.hpp"
#include "test_util.hpp"
#include <iostream>

const std::string TEST_FOLDER = "def";
const std::string COMMENT_TOKEN = "//";
const std::string SHAPE_DELIMITER = "#SHAPE";
const std::string START_END_DELIMITER = "#START_END";
const std::string ADJACENCY_LIST_DELIMITER = "#ADJACENCY_LIST";
const std::string PATH_LENGTH_DELIMITER = "#PATH_LENGTH";
const std::string PATH_POINTS_DELIMITER = "#PATH_POINTS";

std::string get_file_delimiter(const ReadType type) {
    switch (type) {
        case SHAPE:
            return SHAPE_DELIMITER;
        case START_END:
            return START_END_DELIMITER;
        case ADJACENCY_LIST:
            return ADJACENCY_LIST_DELIMITER;
        case PATH_LENGTH:
            return PATH_LENGTH_DELIMITER;
        case PATH_POINTS:
            return PATH_POINTS_DELIMITER;
    }
}

std::string get_next_file_delimiter(const ReadType type) {
    switch (type) {
        case SHAPE:
            return START_END_DELIMITER;
        case START_END:
            return ADJACENCY_LIST_DELIMITER;
        case ADJACENCY_LIST:
            return PATH_LENGTH_DELIMITER;
        case PATH_LENGTH:
            return PATH_POINTS_DELIMITER;
        case PATH_POINTS:
            return "";
    }
}

void* read_test_data(const ReadType type, const std::string& name, const size_t size) {
    Polygon* polygon;
    PointPair* start_end;
    AdjacencyList* adj_list;
    double* path_length;
    std::vector<bfreeman::Point>* path_points;

    switch (type) {
        case SHAPE:
            polygon = new Polygon(size);
            break;
        case START_END:
            start_end = new PointPair;
            break;
        case ADJACENCY_LIST:
            adj_list = new AdjacencyList(size);
            break;
        case PATH_LENGTH:
            path_length = new double;
            break;
        case PATH_POINTS:
            path_points = new std::vector<bfreeman::Point>();
    }

    std::fstream file;
    file.open("./" + TEST_FOLDER + "/" + name, std::ios::in);

    std::string line;
    bool reached = false;

    while (getline(file, line)) {
        line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
        if (line.substr(0, 2) == COMMENT_TOKEN) continue;
        if (line == get_next_file_delimiter(type)) break;
        if (!reached) {
            reached = line == get_file_delimiter(type);
            continue;
        }

        // shape data
        size_t shape_idx;
        double x_shape, y_shape;

        // star_end data
        double x1, y1, x2, y2;

        // adj_list data
        size_t adj_idx, i, j;
        bool interior;
        double distance;

        // path_points data
        double x_path, y_path;
        size_t path_idx = 0;

        size_t str_idx = 0;
        size_t data_idx = 0;
        std::string datum;

        while ((str_idx = line.find(',')) != std::string::npos) {
            datum = line.substr(0, str_idx);
            line.erase(0, str_idx + 1);

            switch (data_idx++) {
                case 0:
                    switch (type) {
                        case SHAPE:
                            shape_idx = std::stoi(datum);
                            break;
                        case START_END:
                            x1 = std::stod(datum);
                            break;
                        case ADJACENCY_LIST:
                            adj_idx = std::stoi(datum);
                            break;
                        case PATH_POINTS:
                            x_path = std::stod(datum);
                            break;
                    }
                    break;
                case 1:
                    switch (type) {
                        case SHAPE:
                            x_shape = std::stod(datum);
                            break;
                        case START_END:
                            y1 = std::stod(datum);
                            break;
                        case ADJACENCY_LIST:
                            i = std::stoi(datum);
                            break;
                    }
                    break;
                case 2:
                    switch (type) {
                        case START_END:
                            x2 = std::stod(datum);
                            break;
                        case ADJACENCY_LIST:
                            j = std::stoi(datum);
                            break;
                    }
                    break;
                case 3:
                    interior = datum == "true";
                    break;
            }
        }
        switch (type) {
            case SHAPE:
                y_shape = std::stod(line);
                push_point((*polygon)[shape_idx], x_shape, y_shape);
                break;
            case START_END:
                y2 = std::stod(line);
                *start_end = {{x1, y1},
                              {x2, y2}};
                break;
            case ADJACENCY_LIST:
                distance = std::stod(line);
                push_edge((*adj_list)[adj_idx], i, j, interior, distance);
                break;
            case PATH_LENGTH:
                *path_length = std::stod(line);
                break;
            case PATH_POINTS:
                y_path = std::stod(line);
                push_point((*path_points), x_path, y_path);
                break;
        }
    }

    file.close();
    switch (type) {
        case SHAPE:
            return (void*) polygon;
        case START_END:
            return (void*) start_end;
        case ADJACENCY_LIST:
            return (void*) adj_list;
        case PATH_LENGTH:
            return (void*) path_length;
        case PATH_POINTS:
            return (void*) path_points;
    }
}