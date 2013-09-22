#pragma once

namespace Nultima
{

class Mouse
{
public:

    enum
    {
        NU_MOUSE_LEFT   = 0,
        NU_MOUSE_MIDDLE,
        NU_MOUSE_RIGHT,
        NU_MOUSE_LAST
    } MouseKeys;

    typedef struct
    {
        bool isDown;
        int x;
        int y;
    } KeyPress;

    Mouse();
    ~Mouse() {};

    void        setClick    (int button, int x, int y, bool isDown);
    void        motion      (int x, int y);
    KeyPress    getKeyPress (int button) { return m_keysDown[button]; }

private:
    KeyPress m_keysDown[NU_MOUSE_LAST];
};

};