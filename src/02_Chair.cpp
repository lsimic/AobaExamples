#include "AobaAPI/AobaAPI.hpp"

#include <cmath>

int main() {
    // create a new mesh
    Aoba::Core::Mesh* mesh = new Aoba::Core::Mesh();

    // use a 3x3 face grid as a starting point
    auto grid = Aoba::Ops::CreateGrid(mesh, 2, 2, 0.5f, 0.5f);

    // find verts which are close to the Y axis, scale them on X axis
     // find them using a lambda expression
    auto vx = mesh->Verts([](const Aoba::Core::Vert* v) { 
        return fabsf(v->co.x) < 0.5f / 3.0f; 
    });
    Aoba::Ops::Scale(mesh, vx, Aoba::Math::Vec3(0.0f, 0.0f, 0.0f), Aoba::Math::Vec3(2.5f, 1.0f, 1.0f));

    // find verts which are close to the X axis, scale them on Y axis
     // find them using a lambda expression
    auto vy = mesh->Verts([](const Aoba::Core::Vert* v) { 
        return fabsf(v->co.y) < 0.5f / 3.0f; 
    });
    Aoba::Ops::Scale(mesh, vy, Aoba::Math::Vec3(0.0f, 0.0f, 0.0f), Aoba::Math::Vec3(1.0f, 2.5f, 1.0f));

    // seat faces - faces which will be extruded to make the seat - all faces currently in the mesh
    auto seatFaces = mesh->Faces();

    // leg faces - faces which will be extruded to make the chair legs - the four corner faces
    // find them using a lambda expression
    auto legFaces = mesh->Faces([](const Aoba::Core::Face* f) {
        return fabsf(f->CalcCenterAverage().x) > 0.01f && fabsf(f->CalcCenterAverage().y) > 0.01f;
    });

    // create the seat by extruding the seat faces and translating the new vertices
    auto seat = Aoba::Ops::ExtrudeFaceRegion(mesh, seatFaces, true);
    Aoba::Ops::Translate(mesh, seat.verts, Aoba::Math::Vec3(0.0f, 0.0f, 0.05f));

    // create the legs by extruding the leg faces and translating the new vertices
    auto legs = Aoba::Ops::ExtrudeFaces(mesh, legFaces, false);
    Aoba::Ops::Translate(mesh, legs.verts, Aoba::Math::Vec3(0.0f, 0.0f, -0.5f));

    // back faces - faces which will be extruded to make the back of the chair
    auto backFaces = mesh->Faces([](const Aoba::Core::Face* f) {
        return f->CalcCenterAverage().y < -0.5f / 3.0f && f->CalcCenterAverage().z > 0.05f / 1.5f;
    });

    // extrude the back faces and translate the vertices to chreate the back of the chair
    auto back = Aoba::Ops::ExtrudeFaceRegion(mesh, backFaces, false);
    Aoba::Ops::Translate(mesh, back.verts, Aoba::Math::Vec3(0.0f, 0.0f, 0.5f));
    
    // export the mesh
    Aoba::IO::ExportObj("chair.obj", mesh);

    return 0;
}