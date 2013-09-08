#include "nuModel.h"
#include "nuDefs.h"
#include "nuContext.h"
#include "nuGraphics.h"
#include "nuTexManager.h"
#include "nuTilemap.h"

using namespace Nultima;

Model::Model(int type) :
    m_indexBuffer(0),
    m_vertexBuffer(0),
    m_numTriangles(0)
{
    switch (type)
    {
    case UNIT_PLANE:
        createUnitPlane();
        //createUnitPlane("forest_heavy");
        break;

    case UNIT_BOX:
        createUnitBox();
        break;

    default:
        NU_ASSERT(!"Unknown model type");
    }
}

void Model::createUnitPlane()
{
    std::vector<Vertex> verts;
    std::vector<Vec3ui> tris;

    Vertex v;
    // Vert 1
    v.coords = Vec3(0,0,0);
    v.normal = Vec3(0,0,1);
    v.texCoord = Vec2(0,0);
    verts.push_back(v);

    // Vert 2
    v.coords = Vec3(1,0,0);
    v.normal = Vec3(0,0,1);
    v.texCoord = Vec2(1,0);
    verts.push_back(v);

    // Vert 3
    v.coords = Vec3(1,1,0);
    v.normal = Vec3(0,0,1);
    v.texCoord = Vec2(1,1);
    verts.push_back(v);

    // Vert 4
    v.coords = Vec3(0,1,0);
    v.normal = Vec3(0,0,1);
    v.texCoord = Vec2(0,1);
    verts.push_back(v);

    tris.push_back(Vec3ui(0, 1, 2));
    tris.push_back(Vec3ui(0, 2, 3));

    Graphics* graphics = Context::get()->getGraphics();
    m_numTriangles = tris.size();
    m_indexBuffer = graphics->createIndexBuffer(&tris[0].m_x, tris.size());
    m_vertexBuffer = graphics->createVertexBuffer(&verts[0].coords.m_x, verts.size());
}

void Model::createUnitPlane(std::string id)
{
    std::vector<Vertex> verts;
    std::vector<Vec3ui> tris;

    Tilemap::TilemapTexture uv = Context::get()->getTilemapTexture(id);

    Vertex v;
    // Vert 1
    v.coords = Vec3(0,0,0);
    v.normal = Vec3(0,0,1);
    v.texCoord = Vec2(uv.x1, uv.y1);
    verts.push_back(v);

    // Vert 2
    v.coords = Vec3(1,0,0);
    v.normal = Vec3(0,0,1);
    v.texCoord = Vec2(uv.x2, uv.y1);
    verts.push_back(v);

    // Vert 3
    v.coords = Vec3(1,1,0);
    v.normal = Vec3(0,0,1);
    v.texCoord = Vec2(uv.x2, uv.y2);
    verts.push_back(v);

    // Vert 4
    v.coords = Vec3(0,1,0);
    v.normal = Vec3(0,0,1);
    v.texCoord = Vec2(uv.x1, uv.y2);
    verts.push_back(v);

    tris.push_back(Vec3ui(0, 1, 2));
    tris.push_back(Vec3ui(0, 2, 3));

    Graphics* graphics = Context::get()->getGraphics();
    m_numTriangles = tris.size();
    m_indexBuffer = graphics->createIndexBuffer(&tris[0].m_x, tris.size());
    m_vertexBuffer = graphics->createVertexBuffer(&verts[0].coords.m_x, verts.size());
}

void Model::createUnitBox()
{
    static const float normals[6][3] =
    {
	    {-1,0,0},
	    {0,1,0},
	    {1,0,0},
	    {0,-1,0},
	    {0,0,1},
	    {0,0,-1}
    };

    static const int faces[6][4] =
    {
	    {0, 1, 2, 3},
	    {3, 2, 6, 7},
	    {7, 6, 5, 4},
	    {4, 5, 1, 0},
	    {5, 6, 2, 1},
	    {7, 4, 0, 3}
    };

    static const float coords[8][3] =
    {
        {0, 0, 0}, //0
        {0, 0, 1}, //1
        {0, 1, 1}, //2
        {0, 1, 0}, //3
        {1, 0, 0}, //4
        {1, 0, 1}, //5
        {1, 1, 1}, //6
        {1, 1, 0}  //7
    };

    std::vector<Vertex> verts;
    std::vector<Vec3ui> tris;

    Vertex v;

    for (int i=0; i<6; i++)
    {
        // Vertex 1
        v.coords = Vec3(coords[faces[i][0]][0],
                        coords[faces[i][0]][1],
                        coords[faces[i][0]][2]);
        v.normal = Vec3(normals[i][0], normals[i][1], normals[i][2]);
        v.texCoord = Vec2(0,0);
        verts.push_back(v);

        // Vertex 2
        v.coords = Vec3(coords[faces[i][1]][0],
                        coords[faces[i][1]][1],
                        coords[faces[i][1]][2]);
        v.normal = Vec3(normals[i][0], normals[i][1], normals[i][2]);
        v.texCoord = Vec2(1,0);
        verts.push_back(v);

        // Vertex 3
        v.coords = Vec3(coords[faces[i][2]][0],
                        coords[faces[i][2]][1],
                        coords[faces[i][2]][2]);
        v.normal = Vec3(normals[i][0], normals[i][1], normals[i][2]);
        v.texCoord = Vec2(1,1);
        verts.push_back(v);

        // Vertex 4
        v.coords = Vec3(coords[faces[i][3]][0],
                        coords[faces[i][3]][1],
                        coords[faces[i][3]][2]);
        v.normal = Vec3(normals[i][0], normals[i][1], normals[i][2]);
        v.texCoord = Vec2(0,1);
        verts.push_back(v);

        tris.push_back(Vec3ui(i*4, i*4+1, i*4+2));
        tris.push_back(Vec3ui(i*4, i*4+2, i*4+3));
    }

    Graphics* graphics = Context::get()->getGraphics();
    m_numTriangles = tris.size();
    m_indexBuffer = graphics->createIndexBuffer(&tris[0].m_x, tris.size());
    m_vertexBuffer = graphics->createVertexBuffer(&verts[0].coords.m_x, verts.size());
}

void Model::render() const
{
    Graphics* graphics = Context::get()->getGraphics();
    graphics->bindIndexBuffer(m_indexBuffer);
    graphics->bindVertexBuffer(m_vertexBuffer);
    graphics->drawElements(m_numTriangles);
}
