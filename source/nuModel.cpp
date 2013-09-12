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
        break;

    case UNIT_BOX:
        createUnitBox();
        break;

    case HALFBOX:
        createUnitHalfBox();
        break;

    case TORUS:
        createTorus(30, 30, 0.5, 0.5);
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
    v.texCoord = Vec2(0,1);
    verts.push_back(v);

    // Vert 2
    v.coords = Vec3(1,0,0);
    v.normal = Vec3(0,0,1);
    v.texCoord = Vec2(1,1);
    verts.push_back(v);

    // Vert 3
    v.coords = Vec3(1,1,0);
    v.normal = Vec3(0,0,1);
    v.texCoord = Vec2(1,0);
    verts.push_back(v);

    // Vert 4
    v.coords = Vec3(0,1,0);
    v.normal = Vec3(0,0,1);
    v.texCoord = Vec2(0,0);
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

void Model::createUnitHalfBox()
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
        {0, 0, 0.5}, //1
        {0, 1, 0.5}, //2
        {0, 1, 0}, //3
        {1, 0, 0}, //4
        {1, 0, 0.5}, //5
        {1, 1, 0.5}, //6
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

void Model::createTorus(int r, int p, float radius, float thickness)
{
	int* pIndices;
	int	nIndices;

	struct Primitive
	{
		int	nIndices;
		int	indexBufferOffset;
		int	indexOffset;
		int	indexRange;
	};

	int			nPrimitives;
	Primitive*	pPrimitives;

	int i, j;

    std::vector<Vertex> verts;

	const float pi  = 3.141592f;
	double rstep    = 1.f/(float)r*2.f*pi;
	double rca      = std::cos(rstep);
	double rsa      = std::sin(rstep);

	float oy = thickness/2;
	float oz = 0.f;

	for(j=0; j<r+1; j++)
	{
		float x = 0.f;
		float y = oy + radius;
		float z = oz;

		float nx = 0.f;
		float ny = oy;
		float nz = oz;

		float pstep = 1.f/float(p) * float(pi)*2.0f;
		double pca = std::cos(pstep);
		double psa = std::sin(pstep);

		for(i=0; i<p; i++)
		{
            Vec3 coords = Vec3(x, y, z);
            Vertex v;
            v.coords = Vec3(x, y, z);

			float bnx = nx;
			float bny = ny;
			float bnz = nz;

			double len = 1.0/std::sqrt(bnx*bnx+bny*bny+bnz*bnz);

            v.normal = Vec3(float(bnx*len), float(bny*len),float(bnz*len)); 

            verts.push_back(v);
			float tx = float(x * pca + y *-psa);
			float ty = float(x * psa + y * pca);
			x = tx;
			y = ty;

			tx = float(nx * pca + ny *-psa);
			ty = float(nx * psa + ny * pca);
			nx = tx;
			ny = ty;
		}

		float ty = float(oy * rca + oz *-rsa);
		float tz = float(oy * rsa + oz * rca);
		oy = ty;
		oz = tz;
	}

	nPrimitives = r;
	pPrimitives = new Primitive[r];

	nIndices = (p+1)*2;
	pIndices = new int[nIndices];

	int* pInd = pIndices;

	for(j=0; j<r; j++)
	{
		pPrimitives[j].nIndices = (p+1)*2;
		pPrimitives[j].indexBufferOffset = 0;
		int indexOffset = j*p;

		pPrimitives[j].indexOffset = indexOffset;
		pPrimitives[j].indexRange = (j+1)*p+p;
		pPrimitives[j].indexRange -= indexOffset;

		if(j == 0)
		{
			for(i=0; i<p; i++)
			{
				*pInd++ = j*p + i - indexOffset;
				*pInd++ = (j+1)*p + i - indexOffset;
			}
			*pInd++ = j*p - indexOffset;
			*pInd++ = (j+1)*p - indexOffset;
		}
	}

	Vec3ui* triangles = new Vec3ui[r*p*2];
    int triangleCount = 0;

	for(int prim=0; prim<nPrimitives; prim++)
	{
		Primitive& p = pPrimitives[prim];

		for(int triangle=0; triangle<p.nIndices-2; triangle++)
		{
			if(triangle & 1)
			{
				triangles[triangleCount] = Vec3ui(
					pIndices[p.indexBufferOffset + triangle + 0] + p.indexOffset,
					pIndices[p.indexBufferOffset + triangle + 1] + p.indexOffset,
					pIndices[p.indexBufferOffset + triangle + 2] + p.indexOffset
				);
			}
			else
			{
				triangles[triangleCount] = Vec3ui(
					pIndices[p.indexBufferOffset + triangle + 0] + p.indexOffset,
					pIndices[p.indexBufferOffset + triangle + 2] + p.indexOffset,
					pIndices[p.indexBufferOffset + triangle + 1] + p.indexOffset
				);
			}
			triangleCount++;
		}
	}

    Graphics* graphics = Context::get()->getGraphics();
    m_numTriangles = triangleCount;

    m_indexBuffer = graphics->createIndexBuffer(&triangles[0].m_x, triangleCount);
    m_vertexBuffer = graphics->createVertexBuffer(&verts[0].coords.m_x, verts.size());

	delete[] pPrimitives;
    delete[] triangles;
    delete[] pIndices;
}

void Model::render() const
{
    Graphics* graphics = Context::get()->getGraphics();
    graphics->bindIndexBuffer(m_indexBuffer);
    graphics->bindVertexBuffer(m_vertexBuffer);
    graphics->drawElements(m_numTriangles);
}
