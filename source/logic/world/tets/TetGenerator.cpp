#include "TetGenerator.h"

#include <string>

#include <glm/gtx/random.hpp>

#include <glowutils/AxisAlignedBoundingBox.h>

#include <3rdparty/tetgen/tetgen.h>


static glm::vec3 getRandomOffset()
{
    return glm::vec3(
        glm::linearRand(0.0f, 0.1f),
        glm::linearRand(0.0f, 0.1f),
        glm::linearRand(0.0f, 0.1f));
}

QVector<Tet *> * TetGenerator::splitBox(const glowutils::AxisAlignedBoundingBox & box)
{
    tetgenio * split = tetcall_main(box);

    QVector<Tet *> * tets = new QVector<Tet *>();

    QVector<glm::vec3> vertices;
    for (int i = 0; i < split->numberofpoints; i++)
        vertices << glm::vec3(split->pointlist[i * 3 + 0], split->pointlist[i * 3 + 1], split->pointlist[i * 3 + 2]);

    for (int tetIndex = 0; tetIndex < split->numberoftetrahedra; tetIndex++) {
        glm::vec3 randomOffset = getRandomOffset();

        QVector<glm::vec3> tetVertices;
        for (int j = 0; j < 4; j++)
            tetVertices << vertices[split->tetrahedronlist[tetIndex * 4 + j]];// +randomOffset;

        tets->append(new Tet(tetVertices));
    }

    return tets;
}


/** taken from http://www.tetgen.org/files/tetcall.cxx */

tetgenio * TetGenerator::tetcall_main(const glowutils::AxisAlignedBoundingBox & box)
{
    tetgenio in;
    tetgenio * out = new tetgenio();
    tetgenio::facet *f;
    tetgenio::polygon *p;
    int i;

    glm::vec3 llf = box.llf();
    glm::vec3 urb = box.urb();

    // All indices start from 1.
    in.firstnumber = 1;

    in.numberofpoints = 8;
    in.pointlist = new REAL[in.numberofpoints * 3];
    in.pointlist[0] = llf.x;  // node 1.
    in.pointlist[1] = llf.y;
    in.pointlist[2] = llf.z;
    in.pointlist[3] = urb.x;  // node 2.
    in.pointlist[4] = llf.y;
    in.pointlist[5] = llf.z;
    in.pointlist[6] = urb.x;  // node 3.
    in.pointlist[7] = urb.y;
    in.pointlist[8] = llf.z;
    in.pointlist[9] = llf.x;  // node 4.
    in.pointlist[10] = urb.y;
    in.pointlist[11] = llf.z;
    // Set node 5, 6, 7, 8.
    for (i = 4; i < 8; i++) {
        in.pointlist[i * 3] = in.pointlist[(i - 4) * 3];
        in.pointlist[i * 3 + 1] = in.pointlist[(i - 4) * 3 + 1];
        in.pointlist[i * 3 + 2] = urb.z;
    }

    in.numberoffacets = 6;
    in.facetlist = new tetgenio::facet[in.numberoffacets];
    in.facetmarkerlist = new int[in.numberoffacets];

    // Facet 1. The leftmost facet.
    f = &in.facetlist[0];
    f->numberofpolygons = 1;
    f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
    f->numberofholes = 0;
    f->holelist = NULL;
    p = &f->polygonlist[0];
    p->numberofvertices = 4;
    p->vertexlist = new int[p->numberofvertices];
    p->vertexlist[0] = 1;
    p->vertexlist[1] = 2;
    p->vertexlist[2] = 3;
    p->vertexlist[3] = 4;

    // Facet 2. The rightmost facet.
    f = &in.facetlist[1];
    f->numberofpolygons = 1;
    f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
    f->numberofholes = 0;
    f->holelist = NULL;
    p = &f->polygonlist[0];
    p->numberofvertices = 4;
    p->vertexlist = new int[p->numberofvertices];
    p->vertexlist[0] = 5;
    p->vertexlist[1] = 6;
    p->vertexlist[2] = 7;
    p->vertexlist[3] = 8;

    // Facet 3. The bottom facet.
    f = &in.facetlist[2];
    f->numberofpolygons = 1;
    f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
    f->numberofholes = 0;
    f->holelist = NULL;
    p = &f->polygonlist[0];
    p->numberofvertices = 4;
    p->vertexlist = new int[p->numberofvertices];
    p->vertexlist[0] = 1;
    p->vertexlist[1] = 5;
    p->vertexlist[2] = 6;
    p->vertexlist[3] = 2;

    // Facet 4. The back facet.
    f = &in.facetlist[3];
    f->numberofpolygons = 1;
    f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
    f->numberofholes = 0;
    f->holelist = NULL;
    p = &f->polygonlist[0];
    p->numberofvertices = 4;
    p->vertexlist = new int[p->numberofvertices];
    p->vertexlist[0] = 2;
    p->vertexlist[1] = 6;
    p->vertexlist[2] = 7;
    p->vertexlist[3] = 3;

    // Facet 5. The top facet.
    f = &in.facetlist[4];
    f->numberofpolygons = 1;
    f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
    f->numberofholes = 0;
    f->holelist = NULL;
    p = &f->polygonlist[0];
    p->numberofvertices = 4;
    p->vertexlist = new int[p->numberofvertices];
    p->vertexlist[0] = 3;
    p->vertexlist[1] = 7;
    p->vertexlist[2] = 8;
    p->vertexlist[3] = 4;

    // Facet 6. The front facet.
    f = &in.facetlist[5];
    f->numberofpolygons = 1;
    f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
    f->numberofholes = 0;
    f->holelist = NULL;
    p = &f->polygonlist[0];
    p->numberofvertices = 4;
    p->vertexlist = new int[p->numberofvertices];
    p->vertexlist[0] = 4;
    p->vertexlist[1] = 8;
    p->vertexlist[2] = 5;
    p->vertexlist[3] = 1;

    // Set 'in.facetmarkerlist'

    in.facetmarkerlist[0] = -1;
    in.facetmarkerlist[1] = -2;
    in.facetmarkerlist[2] = 0;
    in.facetmarkerlist[3] = 0;
    in.facetmarkerlist[4] = 0;
    in.facetmarkerlist[5] = 0;

    double volume = (urb.x - llf.x) * (urb.y - llf.y) * (urb.z - llf.z);
    double maxVolume = volume / 20;
    // Tetrahedralize the PLC. Switches are chosen to read a PLC (p),
    //   do quality mesh generation (q) with a specified quality bound
    //   (1.414), and apply a maximum volume constraint (a0.1). And do it
    //   with zero-based indices (z) because that's what normal people do.
    tetgenbehavior b;
    std::string options = "pq1.414a";
    options += std::to_string(maxVolume);
    options += "z";
    b.parse_commandline(const_cast<char *>(options.data()));

    tetrahedralize(&b, &in, out);

    return out;
}
