#include <vector>
#include "Item.h"
#include "Inventory.h"
#include "gl_utils.h"

Inventory::Inventory()
{
}

void Inventory::addItem(Item* item)
{
    m_items.push_back(item);
}

void removeItem(int id) 
{
    // TODO
}

int Inventory::getTotalWeight()
{
    int weight = 0;

    for (std::vector<Item*>::iterator it = m_items.begin(); it != m_items.end(); ++it)
	weight += (*it)->m_weight;

    return weight;
}

void Inventory::render(int width, int height)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, width, 0.0, height);

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(10.0f, 10.0f);
    glVertex2f(width-10.0f, 10.0f);
    glVertex2f(width-10.0f, height-10.0f);
    glVertex2f(10.0f, height-10.0f);
    glEnd();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}
