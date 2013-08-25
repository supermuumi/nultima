#include <stdlib.h>
#include "Dice.h"

int rollDice(int numSides) 
{
    return rand() % numSides;
}

void rollMultipleDice(int numSides, int numRolls, int* result) 
{
    for (int i = 0; i < numRolls; i++)
	result[i] = rand() % numSides;
}

