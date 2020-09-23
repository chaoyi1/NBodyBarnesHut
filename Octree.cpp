#include "Octree.hpp"
#include <cmath>

Octree::Octree(Vec3d center, double length, double theta)
    : theta{theta}
{
    root = new Node(center, length);
}

Octree::~Octree() { delete root; }

void Octree::insert(Body &body)
{
    insert(root, body);
}

void Octree::calc_accel_for_one(Body &body, double G, double softening)
{
    calc_accel_for_one(root, body, G, softening);
}

void Octree::insert(Node *node, Body &body)
{
    if (node->is_empty() && node->is_leaf())
    {
        node->body = &body;
        node->center_of_mass = body.position;
        node->total_mass = body.mass;
        return;
    }
    else if (node->is_empty() && !node->is_leaf())
    {
        node->center_of_mass.x = ((node->total_mass * node->center_of_mass.x) + (body.mass * body.position.x)) / (node->total_mass + body.mass);
        node->center_of_mass.y = ((node->total_mass * node->center_of_mass.y) + (body.mass * body.position.y)) / (node->total_mass + body.mass);
        node->center_of_mass.z = ((node->total_mass * node->center_of_mass.z) + (body.mass * body.position.z)) / (node->total_mass + body.mass);
        node->total_mass = node->total_mass + body.mass;
        int oct_num = node->get_octant(body.position);
        insert(node->children[oct_num], body);
    }
    else if (!node->is_empty() && node->is_leaf())
    {
        node->divide_octant();
        Body *old_body = node->body;
        node->body = nullptr;
        int oct_num = node->get_octant(body.position);
        int old_oct_num = node->get_octant(old_body->position);
        insert(node->children[oct_num], body);
        insert(node->children[old_oct_num], *old_body);
        node->center_of_mass.x = ((node->total_mass * node->center_of_mass.x) + (body.mass * body.position.x)) / (node->total_mass + body.mass);
        node->center_of_mass.y = ((node->total_mass * node->center_of_mass.y) + (body.mass * body.position.y)) / (node->total_mass + body.mass);
        node->center_of_mass.z = ((node->total_mass * node->center_of_mass.z) + (body.mass * body.position.z)) / (node->total_mass + body.mass);
        node->total_mass = node->total_mass + body.mass;
    }
}

void Octree::calc_accel_for_one(Node *node, Body &body, double G, double softening)
{
    if (node->is_leaf())
    {
        if (node->is_empty())
        {
            body.acceleration.x += 0;
            body.acceleration.y += 0;
            body.acceleration.z += 0;
        }
        else if (node->body == &body)
        {
            body.acceleration.x += 0;
            body.acceleration.y += 0;
            body.acceleration.z += 0;
        }
        else
        {
            double r_ij_x = node->body->position.x - body.position.x;
            double r_ij_y = node->body->position.y - body.position.y;
            double r_ij_z = node->body->position.z - body.position.z;
            double r_ij_norm = std::sqrt(r_ij_x * r_ij_x + r_ij_y * r_ij_y + r_ij_z * r_ij_z);
            double r_ij_norm_sq = r_ij_norm * r_ij_norm;
            double dist_sq = r_ij_norm_sq + (softening * softening);
            double dist = std::sqrt(dist_sq);
            double m_j = node->body->mass;
            body.acceleration.x += (G * m_j * r_ij_x) / (dist * dist * dist);
            body.acceleration.y += (G * m_j * r_ij_y) / (dist * dist * dist);
            body.acceleration.z += (G * m_j * r_ij_z) / (dist * dist * dist);
        }
    }
    else
    {
        double r_ij_x = node->center_of_mass.x - body.position.x;
        double r_ij_y = node->center_of_mass.y - body.position.y;
        double r_ij_z = node->center_of_mass.z - body.position.z;
        double d = std::sqrt(r_ij_x * r_ij_x + r_ij_y * r_ij_y + r_ij_z * r_ij_z);
        double ratio = node->side_length / d;
        if (ratio < theta)
        {
            double r_ij_norm_sq = d * d;
            double dist_sq = r_ij_norm_sq + (softening * softening);
            double dist = std::sqrt(dist_sq);
            double m_j = node->total_mass;
            body.acceleration.x += (G * m_j * r_ij_x) / (dist * dist * dist);
            body.acceleration.y += (G * m_j * r_ij_y) / (dist * dist * dist);
            body.acceleration.z += (G * m_j * r_ij_z) / (dist * dist * dist);
        }
        else
        {
            for (auto &child : node->children)
            {
                if (child != nullptr)
                {
                    calc_accel_for_one(child, body, G, softening);
                }
            }
        }
    }
}