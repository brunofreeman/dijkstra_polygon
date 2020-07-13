#include <vector>
#include <string>
#include "dijkstra_polygon.hpp"
#include "test_util.hpp"
#include "test_data_reader.hpp"
#include <iostream>


int main(int argc, char** argv) {
    bool verbose = argc > 1 && std::strcmp(argv[1], "-v") == 0;

    /*
     * Each polygon and its holes should be defined
     * with counterclockwise winding.
     * 
     * Each adjacency list should list start, end,
     * then the border points in winding order,
     * then each hole in order and its points in
     * winding order.
     * 
     * Within each sublist, the same ordering holds.
     */

    unsigned short passed_tests = 0;

    std::vector<std::string> names;
    std::vector<size_t> polygon_sizes;
    std::vector<size_t> adjacency_list_sizes;

    /*
     * @test     01
     * @name     unit_square_q1
     * @shape    unit square
     * @start    off the bottom left corner
     * @end      off the top right corner
     * @location bottom left corner on origin
     */
    names.emplace_back("unit_sq_q1");
    polygon_sizes.push_back(1);
    adjacency_list_sizes.push_back(6);

    /*
     * @test     02
     * @name     unit_square_all_q
     * @shape    unit square
     * @start    off the bottom left corner
     * @end      off the top right corner
     * @location centered on origin
     */
    names.emplace_back("unit_sq_all_q");
    polygon_sizes.push_back(1);
    adjacency_list_sizes.push_back(6);

    /*
     * @test     03
     * @name     sq_in_sq
     * @shape    unit square with 0.2-side square
     *           hole in the middle
     * @start    off the bottom left corner
     * @end off  the top right corner
     * @location centered on origin
     */
    names.emplace_back("sq_in_sq");
    polygon_sizes.push_back(2);
    adjacency_list_sizes.push_back(10);

    /*
     * @test     04
     * @name     mountain
     * @shape    two-peaked mountain with consistent slope
     *           and evenly spaced peaks
     * @start    within the left peak
     * @end      within the right peak
     * @location bottom left corner on origin
     */
    names.emplace_back("mountain");
    polygon_sizes.push_back(1);
    adjacency_list_sizes.push_back(7);

    /*
     * @test     05
     * @name     irregular_1
     * @shape    very roughly a horizontally flipped
     *           "R" with two triangular holes and a
     *           protrusion with a narrow opening
     *           into the rest of the shape
     * @start    within the the protrusion
     * @end      below one of the triangular holes
     * @location min(x) = 0, min(y) = 0
     */
    names.emplace_back("irregular_1");
    polygon_sizes.push_back(3);
    adjacency_list_sizes.push_back(19);

    /*
     * For each test, reads in data from the test file and
     * also generates the data using dijkstra_polygon.
     * The file and generated results then are compared.
     */
    for (size_t i = 0; i < names.size(); i++) {
        auto* polygon = (Polygon*) read_test_data(SHAPE, names[i], polygon_sizes[i]);
        auto* start_end = (PointPair*) read_test_data(START_END, names[i], 0);

        AdjacencyList test_al = bfreeman::generate_adjacency_list(*polygon, start_end->start, start_end->end);
        auto* true_al = (AdjacencyList*) read_test_data(ADJACENCY_LIST, names[i], adjacency_list_sizes[i]);

        bfreeman::DijkstraData dijkstra_data = bfreeman::dijkstra_path(*polygon, start_end->start, start_end->end);

        auto* true_path_length = (double*) read_test_data(PATH_LENGTH, names[i], 0);

        auto* true_path_points = (std::vector<bfreeman::Point>*)
                read_test_data(PATH_POINTS, names[i], 0);

        run_test(names[i], *polygon, test_al, *true_al, dijkstra_data.distance, *true_path_length,
                 dijkstra_data.path, *true_path_points, passed_tests, verbose);

        delete polygon;
        delete start_end;
        delete true_al;
        delete true_path_length;
        delete true_path_points;
    }

    print_test_report(passed_tests, names.size());

    return 0;
}