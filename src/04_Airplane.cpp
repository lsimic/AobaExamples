#include "AobaAPI/AobaAPI.hpp"

#include <cmath>

// not the prettiest code I've written
int main() {
    // create a new mesh
    Aoba::Core::Mesh* mesh = new Aoba::Core::Mesh();


    // create the fusleage by extruding, translating and scaling a circle
    auto circle = Aoba::Ops::CreateCircle(mesh, 24, 2.75);
    Aoba::Ops::Rotate( mesh, circle.verts, Aoba::Math::Vec3(0, 0, 0), Aoba::Math::Mat3::Rotation(Aoba::Math::Vec3(1, 0, 0), 3.1415926f / 2));
    Aoba::Ops::Translate(mesh, circle.verts, Aoba::Math::Vec3(0, -20, 0));
    std::vector<float> vals = {5.0f, 30.0f, 5.0f};
    std::vector<Aoba::Core::Edge*> last = circle.edges;
    for (int i = 0; i < vals.size(); ++i) {
        auto extrudeResult = Aoba::Ops::ExtrudeEdges(mesh, last);
        Aoba::Ops::Translate(mesh, extrudeResult.verts,
                            Aoba::Math::Vec3(0, vals.at(i), 0));
        last = extrudeResult.horizontalEdges;
    }

    std::vector<float> yTrans = {4.0f, 3.5f, 0.7f};
    std::vector<float> zTrans = {-0.5f, -0.63f, 0.0f};
    std::vector<float> scale = {0.72f, 0.43f, 0.5f};
    float yCent = 0;
    float zCent = 0;
    for (int i = 0; i < yTrans.size(); ++i) {
        auto extrudeResult = Aoba::Ops::ExtrudeEdges(mesh, last);
        Aoba::Ops::Translate(mesh, extrudeResult.verts, Aoba::Math::Vec3(0, yTrans.at(i), zTrans.at(i)));
        last = extrudeResult.horizontalEdges;
        yCent += yTrans.at(i);
        zCent += zTrans.at(i);
        Aoba::Ops::Scale(mesh, extrudeResult.verts, Aoba::Math::Vec3(0, yCent, zCent), Aoba::Math::Vec3(scale.at(i), 1, scale.at(i)));
    }
    Aoba::Ops::MakeFace(mesh, last);

    yTrans = {-13.0f, -0.35f};
    zTrans = {1.6f, 0.0f};
    scale = {0.13f, 0.7f};
    yCent = 0;
    zCent = 0;
    last = circle.edges;
    for (int i = 0; i < yTrans.size(); ++i) {
        auto extrudeResult = Aoba::Ops::ExtrudeEdges(mesh, last);
        Aoba::Ops::Translate(mesh, extrudeResult.verts, Aoba::Math::Vec3(0, yTrans.at(i), zTrans.at(i)));
        last = extrudeResult.horizontalEdges;
        yCent += yTrans.at(i);
        zCent += zTrans.at(i);
        Aoba::Ops::Scale(mesh, extrudeResult.verts, Aoba::Math::Vec3(0, yCent, zCent), Aoba::Math::Vec3(scale.at(i), 1, scale.at(i)));
    }
    Aoba::Ops::MakeFace(mesh, last);


    // create wings by extruding, scaling and translating a circle, and mirroring to create the other side
    circle = Aoba::Ops::CreateCircle(mesh, 24, 6.75);
    Aoba::Ops::Scale(mesh, circle.verts, Aoba::Math::Vec3(0, 0, 0), Aoba::Math::Vec3(0.09f, 1.0f, 1.0f));
    Aoba::Ops::Rotate(mesh, circle.verts, Aoba::Math::Vec3(0, 0, 0), Aoba::Math::Mat3::Rotation(Aoba::Math::Vec3(0, 1, 0), 3.1415926f / 2));
    Aoba::Ops::Translate(mesh, circle.verts, Aoba::Math::Vec3(0, 0, -1.8f));
    auto wingVerts = circle.verts;

    auto extrudeResult = Aoba::Ops::ExtrudeEdges(mesh, circle.edges);
    Aoba::Ops::Translate(mesh, extrudeResult.verts, Aoba::Math::Vec3(3.5f, 0, 0));
    std::vector<Aoba::Core::Face*> facesToDuplicate = extrudeResult.faces;
    wingVerts.insert(wingVerts.end(), extrudeResult.verts.begin(), extrudeResult.verts.end());

    extrudeResult = Aoba::Ops::ExtrudeEdges(mesh, extrudeResult.horizontalEdges);
    Aoba::Ops::Scale(mesh, extrudeResult.verts, Aoba::Math::Vec3(3.5f, 0, -1.8f), Aoba::Math::Vec3(0.15f, 0.15f, 0.15f));
    Aoba::Ops::Translate(mesh, extrudeResult.verts, Aoba::Math::Vec3(25.0f, -12.0f, 0.0f));
    facesToDuplicate.insert(facesToDuplicate.end(), extrudeResult.faces.begin(), extrudeResult.faces.end());
    wingVerts.insert(wingVerts.end(), extrudeResult.verts.begin(), extrudeResult.verts.end());

    auto face = Aoba::Ops::MakeFace(mesh, extrudeResult.horizontalEdges);
    facesToDuplicate.push_back(face);
    // rotate wing
    Aoba::Ops::Rotate( mesh, wingVerts, Aoba::Math::Vec3(0, 0, 0), Aoba::Math::Mat3::Rotation(Aoba::Math::Vec3(0, 1, 0), -0.05f));
    // mirror
    auto mir = Aoba::Ops::Mirror(mesh, {}, {}, facesToDuplicate, Aoba::Math::Vec3(1, 0, 0), Aoba::Math::Vec3(0, 0, 0), true, 0.2f);


    // vertical stabilizer
    circle = Aoba::Ops::CreateCircle(mesh, 24, 4.15f);
    Aoba::Ops::Scale(mesh, circle.verts, Aoba::Math::Vec3(0, 0, 0),  Aoba::Math::Vec3(0.12f, 1, 1));
    wingVerts = circle.verts;

    extrudeResult = Aoba::Ops::ExtrudeEdges(mesh, circle.edges);
    Aoba::Ops::Scale(mesh, extrudeResult.verts, Aoba::Math::Vec3(0, 0, 0), Aoba::Math::Vec3(0.3f, 0.3f, 0.3f));
    Aoba::Ops::Translate(mesh, extrudeResult.verts, Aoba::Math::Vec3(0, -6.6f, 11));
    wingVerts.insert(wingVerts.end(), extrudeResult.verts.begin(), extrudeResult.verts.end());
    facesToDuplicate = extrudeResult.faces;
    face = Aoba::Ops::MakeFace(mesh, extrudeResult.horizontalEdges);
    facesToDuplicate.push_back(face);


    // duplicate vertical stabilizer to make horizontal stabilizer
    auto duplicateResult = Aoba::Ops::Duplicate(mesh, std::vector<Aoba::Core::Vert*>(), std::vector<Aoba::Core::Edge*>(), facesToDuplicate);
    Aoba::Ops::Rotate(mesh, duplicateResult.verts, Aoba::Math::Vec3(0, 0, 0), Aoba::Math::Mat3::Rotation(Aoba::Math::Vec3(0, 1, 0), 1.5));
    wingVerts.insert(wingVerts.end(), duplicateResult.verts.begin(), duplicateResult.verts.end());

    duplicateResult = Aoba::Ops::Duplicate(mesh, std::vector<Aoba::Core::Vert*>(), std::vector<Aoba::Core::Edge*>(), facesToDuplicate);
    Aoba::Ops::Rotate(mesh, duplicateResult.verts, Aoba::Math::Vec3(0, 0, 0), Aoba::Math::Mat3::Rotation(Aoba::Math::Vec3(0, 1, 0), -1.5));
    wingVerts.insert(wingVerts.end(), duplicateResult.verts.begin(), duplicateResult.verts.end());

    Aoba::Ops::Translate(mesh, wingVerts, Aoba::Math::Vec3(0, -26.5f, 1.2f));

    // export the mesh
    Aoba::IO::ExportObj("airplane.obj", mesh);
}