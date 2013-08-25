// class definitions start with upper case
class Poop 
{
public:
    // tab width 4
    Poop();

    // methods start with lowercase
    void stoolSample();

    // class members 
    int m_memberInt;
};

// structs start with uppercase too
struct Doodoo
{
    int crap;
};

// global variables prefixed with g_
int g_globalInt;

void excrement() 
{
    int stool;
    switch (stool)
    {
// preprocessor directives at the beginning of the line
#ifdef ENABLE_LIQUID_STOOL
	case LIQUID: 
	    break;
#endif
    }
}

// functions start with lowercase, then camelcase
// argument alignment
void shiteWipe(int blabla, int doodoo, int craptastic,
	       int blasfbjlbfjls)
{
}
