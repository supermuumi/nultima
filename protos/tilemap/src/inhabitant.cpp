#include "inhabitant.h"
#include "Cell.h"

#include "gl_utils.h"

void Inhabitant::render()
{
    int x = m_cellX*CELL_SIZE + m_x;
    int y = m_cellY*CELL_SIZE + m_y;

    glPushMatrix();
    glColor3f(1.f, 0.f, 0.f);
    cubeAt((float)x, (float)y, 0.5f);
    glPopMatrix();

    // State leaks elsewhere, yikes!
    glColor3f(1.f, 1.f, 1.f);
}
