class Inhabitant
{
public:

    Inhabitant(int cellX, int cellY, int x, int y) : m_cellX(cellX), m_cellY(cellY), m_x(x), m_y(y) {};
    ~Inhabitant() {};

    int getCellX()  { return m_cellX; }
    int getCellY()  { return m_cellY; }
    int getX()      { return m_x; }
    int getY()      { return m_y; }
    void render();

private:

    int m_cellX;
    int m_cellY;
    int m_x;
    int m_y;
};
