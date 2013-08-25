#include <vector>
#include "Item.h"
#include "Inventory.h"

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
