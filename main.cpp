#include "Octree.hpp"
#include <iostream>
#include <random>
#include <cmath>
#include <fstream>

std::vector<Body> generate_bodies(int number_of_bodies, double sim_length, double mean_speed, double mean_mass)
{
    double pi = 3.14159265358979323846;
    std::mt19937_64 generator;
    std::normal_distribution<double> radialDistribution(sim_length / 10., (sim_length / 3.1));
    std::uniform_real_distribution<double> angleDistribution(0.0, (2. * pi));
    std::normal_distribution<double> velDistribution(mean_speed, (mean_speed / 3.));
    std::normal_distribution<double> velAngleDistribution((pi / 2.), (pi / 180.));
    std::normal_distribution<double> massDistribution(mean_mass, (mean_mass / 3.));
    std::uniform_real_distribution<double> heightDistribution(-(sim_length / 2.), (sim_length / 4.));

    std::vector<Body> bodies;
    bodies.reserve(number_of_bodies);

    for (int i = 0; i < number_of_bodies; ++i)
    {
        Body body = Body();

        double r = radialDistribution(generator);
        double rphi = angleDistribution(generator);
        double z = heightDistribution(generator);

        double v = velDistribution(generator);
        double vphi = velAngleDistribution(generator);

        double m = massDistribution(generator);

        body.mass = m;
        body.position = Vec3d(r * cos(rphi), r * sin(rphi), z);
        body.velocity = Vec3d(v * cos(vphi), v * sin(vphi), 0);
        bodies.push_back(body);
    }

    return bodies;
}

void build_tree(std::vector<Body> &bodies, Octree *root)
{
    for (auto &body : bodies)
    {
        root->insert(body);
    }
}

void calc_accel_for_all(std::vector<Body> &bodies, Octree *root, double G, double softening)
{
    for (auto &body : bodies)
    {
        root->calc_accel_for_one(body, G, softening);
    }
}

std::vector<Vec3d> integrate(std::vector<Body> &bodies, Vec3d sim_center, double sim_length, double delta_t, double theta, double G, double softening)
{
    std::vector<Vec3d> position_of_bodies;
    int N = bodies.size();
    position_of_bodies.reserve(N);
    double curr_sim_length = sim_length;

    Octree *root = new Octree(sim_center, curr_sim_length, theta);

    build_tree(bodies, root);
    calc_accel_for_all(bodies, root, G, softening);

    for (auto &body : bodies)
    {
        body.velocity.x += body.acceleration.x * delta_t * 0.5;
        body.velocity.y += body.acceleration.y * delta_t * 0.5;
        body.velocity.z += body.acceleration.z * delta_t * 0.5;
        body.position.x += body.velocity.x * delta_t;
        body.position.y += body.velocity.y * delta_t;
        body.position.z += body.velocity.z * delta_t;
        body.reset_accel();
        position_of_bodies.push_back(body.position);
        double max_length = std::max(std::abs(body.position.z), std::max(std::abs(body.position.x), std::abs(body.position.y)));
        curr_sim_length = std::max(curr_sim_length, 2.2 * max_length);
    }

    delete root;

    root = new Octree(sim_center, curr_sim_length, theta);

    build_tree(bodies, root);
    calc_accel_for_all(bodies, root, G, softening);

    for (auto &body : bodies)
    {
        body.velocity.x += body.acceleration.x * delta_t * 0.5;
        body.velocity.y += body.acceleration.y * delta_t * 0.5;
        body.velocity.z += body.acceleration.z * delta_t * 0.5;
        body.reset_accel();
    }
    delete root;

    return position_of_bodies;
}

std::vector<std::vector<Vec3d>> run_simulation()
{
    int N = 1000;
    double sim_length = 5e12;
    Vec3d sim_center = Vec3d();
    double mean_speed = 10e3;
    double mean_mass = 1.989e30;
    int time_steps = 50000;
    double time_step_size = 3e3;
    double softening_factor = 1.3e12;
    double G = 6.67e-11;
    double theta = 0.5;

    std::vector<Body> bodies = generate_bodies(N, sim_length, mean_speed, mean_mass);

    std::vector<std::vector<Vec3d>> sim_data;
    sim_data.reserve(time_steps);

    for (int t = 0; t < time_steps; ++t)
    {
        std::vector<Vec3d> pos_of_bodies = integrate(bodies, sim_center, sim_length, time_step_size, theta, G, softening_factor);
        for (auto &position : pos_of_bodies)
        {
            double max_length = std::max(std::abs(position.z), std::max(std::abs(position.x), std::abs(position.y)));
            sim_length = std::max(sim_length, 2.2 * max_length);
        }
        sim_data.push_back(pos_of_bodies);
    }
    return sim_data;
}

int main(int argc, char const *argv[])
{
    std::vector<std::vector<Vec3d>> data = run_simulation();
    std::ofstream Simulation_Data("NBody_BH_data.txt");
    for (auto &pos_of_bodies : data)
    {
        for (int i = 0; i < pos_of_bodies.size(); ++i)
        {
            if (i == pos_of_bodies.size() - 1)
            {
                Simulation_Data << pos_of_bodies[i].x << "," << pos_of_bodies[i].y << "," << pos_of_bodies[i].z << "\n";
            }
            else
            {
                Simulation_Data << pos_of_bodies[i].x << "," << pos_of_bodies[i].y << "," << pos_of_bodies[i].z << ",";
            }
        }
    }
    Simulation_Data.close();
    return 0;
}
