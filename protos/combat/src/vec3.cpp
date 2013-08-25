#include <math.h>
#include "vec3.h"

Vec3::Vec3() { x = y = z = .0f; }
Vec3::Vec3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }

Vec3 Vec3::operator+ (Vec3 v) {
    Vec3 ret;
    ret.x = x + v.x;
    ret.y = y + v.y;
    ret.z = z + v.z;
    return ret;
}

void Vec3::operator+= (Vec3 v) {
    x += v.x;
    y += v.y;
    z += v.z;
}

Vec3 Vec3::operator- (Vec3 v) {
    Vec3 ret;
    ret.x = x - v.x;
    ret.y = y - v.y;
    ret.z = z - v.z;
    return ret;
}

void Vec3::operator-= (Vec3 v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
}

Vec3 Vec3::operator* (float n) {
    Vec3 ret;
    ret.x = x * n;
    ret.y = y * n;
    ret.z = z * n;
    return ret;
}

float Vec3::length() {
    return sqrtf(x*x + y*y + z*z);
}

Vec3 Vec3::unit() {
    Vec3 ret;
    float l = this->length();
    ret.x = x/l;
    ret.y = y/l;
    ret.z = z/l;
    return ret;
}

float Vec3::dot(Vec3 v) {
    return x*v.x + y*v.y + z*v.z;
}

Vec3 Vec3::cross(Vec3 v) {
    Vec3 ret;

    ret.x = y*v.z - z*v.y;
    ret.y = z*v.x - x*v.z;
    ret.z = x*v.y - y*v.x;

    return ret;
}

bool Vec3::operator== (Vec3 v)
{
    return (x == v.x && y == v.y && z == v.z);
}