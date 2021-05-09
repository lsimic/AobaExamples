#include "AobaAPI/AobaAPI.hpp"

int main() {
    // crate a new mesh
    Aoba::Core::Mesh* mesh = new Aoba::Core::Mesh();

    // create a new vertex inside the mesh
    Aoba::Core::Vert* vert = Aoba::Ops::CreateVert(mesh, Aoba::Math::Vec3(-1, -1, -1));

    // extrude the vertex into an edge, translate the new vertex
    auto evResult = Aoba::Ops::ExtrudeVerts(mesh, {vert});
    Aoba::Ops::Translate(mesh, evResult.verts, Aoba::Math::Vec3(2, 0, 0));

    // extrude the edge from the previous step into a face, translate new vertices
    auto eeResult = Aoba::Ops::ExtrudeEdges(mesh, evResult.edges);
    Aoba::Ops::Translate(mesh, eeResult.verts, Aoba::Math::Vec3(0, 2, 0));

    // extrude the face from the previous step to create a volume(cube), translate new vertices
    auto efResult = Aoba::Ops::ExtrudeFaces(mesh, eeResult.faces, true);
    Aoba::Ops::Translate(mesh, efResult.verts, Aoba::Math::Vec3(0, 0, 2));

    // export the mesh
    Aoba::IO::ExportObj("cube.obj", mesh);

    return 0;
}
