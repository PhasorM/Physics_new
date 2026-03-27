#pragma once

#include <vector>
#include "raylib.h"

struct Point { float x; float y; };

struct Boundary {
    float x, y, w, h;
    bool contains(Point p) const {
        return (p.x >= x - w && p.x <= x + w && p.y >= y - h && p.y <= y + h);
    }
    bool intersects(Boundary range) const {
        return !(range.x - range.w > x + w || range.x + range.w < x - w ||
                 range.y - range.h > y + h || range.y + range.h < y - h);
    }
};

class QuadTree; 


// QuadtreeAllocator contains a pool of quadtrees and allocates them to each node as needed in O(1) time. It also has a reset function that "deletes" all nodes in O(1) time by simply resetting the index.
class QuadTreeAllocator {
public:
    std::vector<QuadTree> pool;
    int nextAvailable;
    QuadTreeAllocator(int maxNodes);
    void reset();
    QuadTree* allocate(Boundary b, int cap);
};


class QuadTree {
public:
    Boundary boundary;
    int capacity;
    std::vector<Point> points;
    bool divided;

    QuadTree* northwest;
    QuadTree* northeast;
    QuadTree* southwest;
    QuadTree* southeast;

    QuadTreeAllocator* allocator;

    QuadTree() = default; // Default for the pool
    QuadTree(Boundary b, int cap, QuadTreeAllocator* alloc);

    void reset(Boundary b, int cap, QuadTreeAllocator* alloc);
    void subdivide();
    bool insert(Point p);
    std::vector<Point> query(Boundary range);
    void show();
};