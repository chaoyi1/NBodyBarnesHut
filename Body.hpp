#include "Vec3d.hpp"

#ifndef BODY_H
#define BODY_H

class Body
{
public:
    Body();
    Body(double m, Vec3d pos, Vec3d vel, Vec3d acc);
    void reset_accel();

    double mass;
    Vec3d position;
    Vec3d velocity;
    Vec3d acceleration;
};

#endif