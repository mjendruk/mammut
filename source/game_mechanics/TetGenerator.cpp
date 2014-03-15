#include "TetGenerator.h"

#include <glm/gtx/random.hpp>

#include <3rdparty/tetgen/tetgen.h>

#include "Cuboid.h"

static glm::vec3 getRandomOffset()
{
    return glm::vec3(
        glm::linearRand(0.0f, 0.1f),
        glm::linearRand(0.0f, 0.1f),
        glm::linearRand(0.0f, 0.1f));
}

QVector<glm::vec3> TetGenerator::split()
{
    tetgenio * split = tetcall_main();

    glow::Array<glm::vec3> vertices;
    glow::Array<glm::vec3> duplicatedVertices;

    for (int i = 0; i < split->numberofpoints; i++) {
        vertices << glm::vec3(split->pointlist[i * 3 + 0], split->pointlist[i * 3 + 1], split->pointlist[i * 3 + 2]);
    }

    for (int tetIndex = 0; tetIndex < split->numberoftetrahedra; tetIndex++) {
        glm::vec3 randomOffset = getRandomOffset();

        glow::Array<glm::vec3> tetVertices;
        for (int j = 0; j < 4; j++)
            tetVertices << vertices[split->tetrahedronlist[tetIndex * 4 + j] - 1];// +randomOffset;

        glm::vec3 tetCenter = tetVertices[0] + tetVertices[1] + tetVertices[2] + tetVertices[3];
        tetCenter /= 4;
        
        glm::vec3 expandOffset = tetCenter;
        expandOffset *= 1.0;
        for (int j = 0; j < 4; j++)
            tetVertices[j] += expandOffset;
        tetCenter += expandOffset;
        
        for (int j = 0; j < 4; j++) {
            glow::Array<glm::vec3> tri;
            for (int k = 0; k < 4; k++) {
                if (k == j)
                    continue;
                tri << tetVertices[k];
            }
            glm::vec3 triCenter = (tri[0] + tri[1] + tri[2]);
            triCenter /= 3;

            //if the normal is pointing in the wrong direction, reverse the vertices order
            glm::vec3 a = tri.at(0) - tri.at(1);
            glm::vec3 b = tri.at(2) - tri.at(1);
            glm::vec3 normal = glm::normalize(glm::cross(a, b));
            normal *= 0.0001;

            if (glm::distance(triCenter, tetCenter) > glm::distance(triCenter + normal, tetCenter))
                std::swap(tri[0], tri[2]);

            duplicatedVertices << tri[0];
            duplicatedVertices << tri[1];
            duplicatedVertices << tri[2];
        }
    }

    return duplicatedVertices;
}

/** taken from http://www.tetgen.org/files/tetcall.cxx */

tetgenio * TetGenerator::tetcall_main()
{
    tetgenio in;
    tetgenio * out = new tetgenio();
    tetgenio::facet *f;
    tetgenio::polygon *p;
    int i;

    // All indices start from 1.
    in.firstnumber = 1;

    in.numberofpoints = 8;
    in.pointlist = new REAL[in.numberofpoints * 3];
    in.pointlist[0] = -0.5;  // node 1.
    in.pointlist[1] = -0.5;
    in.pointlist[2] = -0.5;
    in.pointlist[3] = 0.5;  // node 2.
    in.pointlist[4] = -0.5;
    in.pointlist[5] = -0.5;
    in.pointlist[6] = 0.5;  // node 3.
    in.pointlist[7] = 0.5;
    in.pointlist[8] = -0.5;
    in.pointlist[9] = -0.5;  // node 4.
    in.pointlist[10] = 0.5;
    in.pointlist[11] = -0.5;
    // Set node 5, 6, 7, 8.
    for (i = 4; i < 8; i++) {
        in.pointlist[i * 3] = in.pointlist[(i - 4) * 3];
        in.pointlist[i * 3 + 1] = in.pointlist[(i - 4) * 3 + 1];
        in.pointlist[i * 3 + 2] = 0.5;
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

    // Output the PLC to files 'barin.node' and 'barin.poly'.
    //in.save_nodes("barin");
    //in.save_poly("barin");

    // Tetrahedralize the PLC. Switches are chosen to read a PLC (p),
    //   do quality mesh generation (q) with a specified quality bound
    //   (1.414), and apply a maximum volume constraint (a0.1).

    tetgenbehavior b;
    b.parse_commandline("pq1.414a0.1");

    tetrahedralize(&b, &in, out);

    // Output mesh to files 'barout.node', 'barout.ele' and 'barout.face'.
    out->save_nodes("barout");
    out->save_elements("barout");
    out->save_faces("barout");
    out->save_poly("barout");
    out->save_faces2smesh("barout");

    return out;
}