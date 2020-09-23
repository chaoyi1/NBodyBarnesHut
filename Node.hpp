#include "Body.hpp"

#ifndef NODE_H
#define NODE_H

class Node
{
public:
    Node(Vec3d cen, double length);
    bool is_empty();
    bool is_leaf();
    int get_octant(Vec3d &body_pos);
    void divide_octant();
    ~Node();

    double total_mass;
    Vec3d center_of_mass;
    Vec3d center;
    double side_length;
    Body *body;
    Node *children[8];
};

#endif