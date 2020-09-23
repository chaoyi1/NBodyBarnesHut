#include "Body.hpp"

Body::Body()
    : mass{0.}, position{Vec3d()}, velocity{Vec3d()}, acceleration{Vec3d()}
{
}

Body::Body(double m, Vec3d pos, Vec3d vel, Vec3d acc)
    : mass{m}, position{pos}, velocity{vel}, acceleration{acc}
{
}

void Body::reset_accel()
{
    acceleration = Vec3d();
}