#include "Node.hpp"

#ifndef OCTREE_H
#define OCTREE_H

class Octree
{
public:
    Octree(Vec3d center, double length, double theta);
    void insert(Body &body);
    void calc_accel_for_one(Body &body, double G, double softening);
    Node *root;
    double theta;
    ~Octree();

private:
    void insert(Node *node, Body &body);
    void calc_accel_for_one(Node *node, Body &body, double G, double softening);
};

#endif