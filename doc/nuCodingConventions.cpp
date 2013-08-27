// Filenames are prefixed with nu and written in mixed caps

// Preprocessor macros are prefixed with NU_
#define NU_UNDEF(X) (void)(X)

// The namespace is Nultima
namespace Nultima
{

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

}; // namespace