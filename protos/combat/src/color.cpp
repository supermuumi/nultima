#include "color.h"

Color::Color() {
    r = g = b = .0f;
    a = 1.0f;
}

Color::Color(float _r, float _g, float _b, float _a) {
    r = _r;
    g = _g;
    b = _b;
    a = _a;
}

void Color::getFloats(float* arr) {
    arr[0] = r;
    arr[1] = g;
    arr[2] = b;
    arr[3] = a;
}
