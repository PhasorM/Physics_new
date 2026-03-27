#include "quadtree.h"

QuadTreeAllocator::QuadTreeAllocator(int maxNodes) {
    pool.resize(maxNodes); 
    nextAvailable = 0;
}

void QuadTreeAllocator::reset() {
    nextAvailable = 0;
}

QuadTree* QuadTreeAllocator::allocate(Boundary b, int cap) {
    if (nextAvailable >= pool.size()) {
        pool.resize(pool.size() * 2); 
    }
    
    // Grab the next node and reset its data
    QuadTree* node = &pool[nextAvailable++];
    node->reset(b, cap, this);
    return node;
}


QuadTree::QuadTree(Boundary b, int cap, QuadTreeAllocator* alloc) {
    reset(b, cap, alloc);
}

void QuadTree::reset(Boundary b, int cap, QuadTreeAllocator* alloc) {
    boundary = b;
    capacity = cap;
    allocator = alloc;
    points.clear();
    divided = false;
    northwest = nullptr;
    northeast = nullptr;
    southwest = nullptr;
    southeast = nullptr;
}

void QuadTree::subdivide() {
    float x = boundary.x;
    float y = boundary.y;
    float w = boundary.w;
    float h = boundary.h;

    Boundary nw = {x - w/2, y - h/2, w/2, h/2};
    Boundary ne = {x + w/2, y - h/2, w/2, h/2};
    Boundary sw = {x - w/2, y + h/2, w/2, h/2};
    Boundary se = {x + w/2, y + h/2, w/2, h/2};

    northwest = allocator->allocate(nw, capacity);
    northeast = allocator->allocate(ne, capacity);
    southwest = allocator->allocate(sw, capacity);
    southeast = allocator->allocate(se, capacity);

    divided = true;
}

bool QuadTree::insert(Point p) {
    if (!boundary.contains(p)) return false;

    if (points.size() < capacity && !divided) {
        points.push_back(p);
        return true;
    }

    if (!divided) subdivide();

    if (northwest->insert(p)) return true;
    if (northeast->insert(p)) return true;
    if (southwest->insert(p)) return true;
    if (southeast->insert(p)) return true;

    return false;
}

std::vector<Point> QuadTree::query(Boundary range) {
    std::vector<Point> found;
    if (!boundary.intersects(range)) return found;

    for (const auto& p : points) {
        if (range.contains(p)) found.push_back(p);
    }

    if (divided) {
        auto nwFound = northwest->query(range);
        found.insert(found.end(), nwFound.begin(), nwFound.end());
        
        auto neFound = northeast->query(range);
        found.insert(found.end(), neFound.begin(), neFound.end());
        
        auto swFound = southwest->query(range);
        found.insert(found.end(), swFound.begin(), swFound.end());
        
        auto seFound = southeast->query(range);
        found.insert(found.end(), seFound.begin(), seFound.end());
    }

    return found;
}

void QuadTree::show() {
    DrawRectangleLines(
        (int)(boundary.x - boundary.w), 
        (int)(boundary.y - boundary.h), 
        (int)(boundary.w * 2), 
        (int)(boundary.h * 2), 
        WHITE
    );

    if (divided) {
        northwest->show();
        northeast->show();
        southwest->show();
        southeast->show();
    }
}