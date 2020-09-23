#include "Node.hpp"

Node::Node(Vec3d cen, double length)
    : total_mass{0.}, center_of_mass{Vec3d()}, center{cen}, side_length{length}, body{nullptr}, children()
{
}

Node::~Node()
{
    for (auto &child : children)
    {
        if (child != nullptr)
        {
            delete child;
        }
    }
}

bool Node::is_empty()
{
    return body == nullptr;
}

bool Node::is_leaf()
{
    return children[0] == nullptr;
}

int Node::get_octant(Vec3d &body_pos)
{
    int oct = -1;
    if (body_pos.x > center.x && body_pos.y > center.y && body_pos.z < center.z)
    {
        oct = 0;
    }
    else if (body_pos.x < center.x && body_pos.y > center.y && body_pos.z < center.z)
    {
        oct = 1;
    }
    else if (body_pos.x < center.x && body_pos.y < center.y && body_pos.z < center.z)
    {
        oct = 2;
    }
    else if (body_pos.x > center.x && body_pos.y < center.y && body_pos.z < center.z)
    {
        oct = 3;
    }
    else if (body_pos.x > center.x && body_pos.y > center.y && body_pos.z > center.z)
    {
        oct = 4;
    }
    else if (body_pos.x < center.x && body_pos.y > center.y && body_pos.z > center.z)
    {
        oct = 5;
    }
    else if (body_pos.x < center.x && body_pos.y < center.y && body_pos.z > center.z)
    {
        oct = 6;
    }
    else
    {
        oct = 7;
    }

    return oct;
}

void Node::divide_octant()
{
    double d = side_length / 4.0;

    Vec3d v0 = Vec3d(center.x + d, center.y + d, center.z - d);
    children[0] = new Node(v0, 2.0 * d);

    Vec3d v1 = Vec3d(center.x - d, center.y + d, center.z - d);
    children[1] = new Node(v1, 2.0 * d);

    Vec3d v2 = Vec3d(center.x - d, center.y - d, center.z - d);
    children[2] = new Node(v2, 2.0 * d);

    Vec3d v3 = Vec3d(center.x + d, center.y - d, center.z - d);
    children[3] = new Node(v3, 2.0 * d);

    Vec3d v4 = Vec3d(center.x + d, center.y + d, center.z + d);
    children[4] = new Node(v4, 2.0 * d);

    Vec3d v5 = Vec3d(center.x - d, center.y + d, center.z + d);
    children[5] = new Node(v5, 2.0 * d);

    Vec3d v6 = Vec3d(center.x - d, center.y - d, center.z + d);
    children[6] = new Node(v6, 2.0 * d);

    Vec3d v7 = Vec3d(center.x + d, center.y - d, center.z + d);
    children[7] = new Node(v7, 2.0 * d);
}