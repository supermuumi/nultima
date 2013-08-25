#ifndef __COLOR_HPP__
#define __COLOR_HPP__

class Color {
public:
    Color();
    Color(float,float,float,float);
    void getFloats(float* arr);

    float r, g, b, a;
};

#endif
