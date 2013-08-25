#ifndef __VECTOR_HPP__
#define __VECTOR_HPP__

class Vec3 {

public:
    Vec3();
    Vec3(float,float,float);

    Vec3 operator+ (Vec3);
    void operator+= (Vec3);
    Vec3 operator- (Vec3);
    void operator-= (Vec3);
    Vec3 operator* (float);
    bool operator== (Vec3);

    float length();
    Vec3 unit();
    float dot(Vec3);
    Vec3 cross(Vec3);

    float x; 
    float y; 
    float z; 
};

#endif
