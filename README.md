# dijkstra_polygon
## Functionality
`dijkstra_polygon` finds the shortest interior path between two points in an arbitrary non-self-intersecting polygon with holes by constructing the adjacency list of the polygon graph and applying Dijkstra's algorithm.

## Usage
The most important function provided is `dijkstra_path`, which has the following function signature:
```cpp
DijkstraData dijkstra_path(
        const std::vector<std::vector<Point>>& polygon,
        const Point& start,
        const Point& end
);
```
The `DijkstraData` and `Point` structs are defined in `dijkstra_polygon.hpp`:
```cpp
struct Point {
    double x;
    double y;
};

struct DijkstraData {
    std::vector<Point> path;
    double distance;
};
```
`polygon` must define a non-self-intersecting polygon with holes. The first `std::vector<Point>` in `polygon` must define the boundary of the polygon. Subsequent `std::vector<Point>` define the holes of the polygon. Both the boundary and the holes must be defined with counterclockwise winding. None of the holes should overlap. `start` and `end` should both be valid interior points of `polygon`.

`path[0] == start` and `path[path.size() - 1] == end` (by value, not reference). Intermediary points, if any, define the shortest interior path between `start` and `end`.

## Example
```cpp
auto point = [](double x, double y) {
    return (bfreeman::Point){x, y};
};

// the polygon has a boundary and two holes
std::vector<std::vector<bfreeman::Point>> polygon(3);

// the boundary is a square
polygon[0].push_back(point(-5, -5));
polygon[0].push_back(point( 5, -5));
polygon[0].push_back(point( 5,  5));
polygon[0].push_back(point(-5,  5));

// one hole is a triangle in the lower left corner
polygon[1].push_back(point(-4, -4));
polygon[1].push_back(point(-3, -4));
polygon[1].push_back(point(-4, -3));

// the other hole is an irregular pentagon in the upper right corner
polygon[2].push_back(point(4, 4));
polygon[2].push_back(point(2, 3));
polygon[2].push_back(point(1, 2));
polygon[2].push_back(point(2, 1));
polygon[2].push_back(point(3, 3));

// the start point is between the triangular hole and the lower left boundary corner
bfreeman::Point start = point(-4.5, -4.5);

// the end point is near the middle of the left boundary edge
bfreeman::Point end = point(4, 0);

bfreeman::DijkstraData dd = dijkstra_path(polygon, start, end);

for (size_t i = 0; i < dd.path.size(); i++) {
    std::cout << "(" << dd.path[i].x << ", "
                     << dd.path[i].y << ")";
    if (i < dd.path.size() - 1) std::cout << ", ";
}
std::cout << ": " << dd.distance << std::endl;
```

This code will print `(-4.5, -4.5), (-3, -4), (4, 0): 9.6434`, signifying that the shortest interior path between `start` and `end` requires navigation around the lower right corner of the triangular hole and that the total path length is `9.6434` units.