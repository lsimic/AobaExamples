#include "AobaAPI/AobaAPI.hpp"

#include <random>
#include <sstream>
#include <iostream>
#include <cmath>

// chair params class, which holds the properties which define the chair
class ChairParams {
  public:
    float width;
    float depth;
    float seatHeight;
    float seatThickness;
    float backHeight;
    float legScale;
    bool hasBack;

    ChairParams(int seed) {
        std::default_random_engine generator(seed);
        std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
        width = 0.4f + distribution(generator) * 0.2f;
        depth = 0.4f + distribution(generator) * 0.2f;
        seatHeight = 0.4f + distribution(generator) * 0.2f;
        seatThickness = 0.02f + distribution(generator) * 0.06f;
        backHeight = 0.5f + distribution(generator) * 0.4f;
        legScale = 2.0f + distribution(generator) * 1.0f;
        hasBack = (distribution(generator) > 0.5f) ? true : false;
    }
};

// extract the chair generation into a separate function
Aoba::Core::Mesh* MakeChair(ChairParams params) {
    // create a new mesh
    Aoba::Core::Mesh* mesh = new Aoba::Core::Mesh();

    // use a 3x3 face grid as a starting point
    auto grid = Aoba::Ops::CreateGrid(mesh, 2, 2, params.width, params.depth);

    // find verts which are close to the Y axis, scale them on X axis
     // find them using a lambda expression
    auto vx = mesh->Verts([params](const Aoba::Core::Vert* v) { 
        return fabsf(v->co.x) < params.width / 3.0f; 
    });
    Aoba::Ops::Scale(mesh, vx, Aoba::Math::Vec3(0.0f, 0.0f, 0.0f), Aoba::Math::Vec3(params.legScale, 1.0f, 1.0f));

    // find verts which are close to the X axis, scale them on Y axis
     // find them using a lambda expression
    auto vy = mesh->Verts([params](const Aoba::Core::Vert* v) { 
        return fabsf(v->co.y) < params.depth / 3.0f; 
    });
    Aoba::Ops::Scale(mesh, vy, Aoba::Math::Vec3(0.0f, 0.0f, 0.0f), Aoba::Math::Vec3(1.0f, params.legScale, 1.0f));

    // seat faces - faces which will be extruded to make the seat - all faces currently in the mesh
    auto seatFaces = mesh->Faces();

    // leg faces - faces which will be extruded to make the chair legs - the four corner faces
    // find them using a lambda expression
    auto legFaces = mesh->Faces([](const Aoba::Core::Face* f) {
        return fabsf(f->CalcCenterAverage().x) > 0.01f && fabsf(f->CalcCenterAverage().y) > 0.01f;
    });

    // create the seat by extruding the seat faces and translating the new vertices
    auto seat = Aoba::Ops::ExtrudeFaceRegion(mesh, seatFaces, true);
    Aoba::Ops::Translate(mesh, seat.verts, Aoba::Math::Vec3(0.0f, 0.0f, params.seatThickness));

    // create the legs by extruding the leg faces and translating the new vertices
    auto legs = Aoba::Ops::ExtrudeFaces(mesh, legFaces, false);
    Aoba::Ops::Translate(mesh, legs.verts, Aoba::Math::Vec3(0.0f, 0.0f, -params.seatHeight));

    // will the back of the chair be generated or not?
    if(params.hasBack) {
        // back faces - faces which will be extruded to make the back of the chair
        auto backFaces = mesh->Faces([params](const Aoba::Core::Face* f) {
            return f->CalcCenterAverage().y < -params.depth / 3.0f && f->CalcCenterAverage().z > params.seatThickness / 1.5f;
        });

        // extrude the back faces and translate the vertices to chreate the back of the chair
        auto back = Aoba::Ops::ExtrudeFaceRegion(mesh, backFaces, false);
        Aoba::Ops::Translate(mesh, back.verts, Aoba::Math::Vec3(0.0f, 0.0f, params.backHeight));
    }

    // move the mesh upwards so that legs stand on the x-y plane
    Aoba::Ops::Translate(mesh, mesh->Verts(), Aoba::Math::Vec3(0, 0, params.seatHeight));

    // return the resulting mesh
    return mesh;
}

int main() {
    for(int i = 0; i < 200; ++i) {
        ChairParams params = ChairParams(i);
        if(params.hasBack) {
            std::cout<<i<< " ";
        }
    }
    /*

    
    // prompt the user to enter the seed
    int seed;
    std::cout<<"Enter random seed: ";
    std::cin>>seed;
    
    // generate params using the seed, generate the chair mesh using the params
    ChairParams params = ChairParams(seed);
    auto mesh = MakeChair(params);

    // create filename using the seed
    std::ostringstream stringStream;
    stringStream << "chair_";
    stringStream << seed;
    stringStream << ".obj";
    std::string name = stringStream.str();

    // export mesh, kill mesh(free the memory)
    Aoba::IO::ExportObj(name, mesh);
    Aoba::Core::KillMesh(mesh);
    */
    return 0;
}
