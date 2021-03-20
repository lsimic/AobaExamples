#include "AobaAPI/AobaAPI.hpp"

int main() {
  // rejoice in my questionable modelling technique and obfuscated naming scheme
  // :)

  Aoba::Core::Mesh* mesh = new Aoba::Core::Mesh();

  // create fuselage by extruding, translating and scaling a circle
  auto circle = Aoba::Ops::CreateCircle(mesh, 24, 2.75);

  Aoba::Ops::Rotate(
      mesh, circle.verts, Aoba::Math::Vec3(0, 0, 0),
      Aoba::Math::Mat3::Rotation(Aoba::Math::Vec3(1, 0, 0), 3.1415926 / 2));

  Aoba::Ops::Translate(mesh, circle.verts, Aoba::Math::Vec3(0, -20, 0));

  std::vector<int> vals = {5, 30, 5};
  std::vector<Aoba::Core::Edge*> last = circle.edges;
  for (int i = 0; i < vals.size(); ++i) {
    auto extrudeResult = Aoba::Ops::ExtrudeEdges(mesh, last);
    Aoba::Ops::Translate(mesh, extrudeResult.verts,
                         Aoba::Math::Vec3(0, vals.at(i), 0));
    last = extrudeResult.horizontalEdges;
  }

  // front part of fuselage
  std::vector<float> yTrans = {4, 3.5, 0.7};
  std::vector<float> zTrans = {-0.5, -0.63, 0};
  std::vector<float> scale = {0.72, 0.43, 0.5};
  float yCent = 0;
  float zCent = 0;
  for (int i = 0; i < yTrans.size(); ++i) {
    auto extrudeResult = Aoba::Ops::ExtrudeEdges(mesh, last);
    Aoba::Ops::Translate(mesh, extrudeResult.verts,
                         Aoba::Math::Vec3(0, yTrans.at(i), zTrans.at(i)));
    last = extrudeResult.horizontalEdges;
    yCent += yTrans.at(i);
    zCent += zTrans.at(i);
    Aoba::Ops::Scale(mesh, extrudeResult.verts,
                     Aoba::Math::Vec3(0, yCent, zCent),
                     Aoba::Math::Vec3(scale.at(i), 1, scale.at(i)));
  }
  Aoba::Ops::MakeFace(mesh, last);

  // rear part of fuselage
  yTrans = {-13, -0.35};
  zTrans = {1.6, 0};
  scale = {0.13, 0.7};
  yCent = 0;
  zCent = 0;
  last = circle.edges;
  for (int i = 0; i < yTrans.size(); ++i) {
    auto extrudeResult = Aoba::Ops::ExtrudeEdges(mesh, last);
    Aoba::Ops::Translate(mesh, extrudeResult.verts,
                         Aoba::Math::Vec3(0, yTrans.at(i), zTrans.at(i)));
    last = extrudeResult.horizontalEdges;
    yCent += yTrans.at(i);
    zCent += zTrans.at(i);
    Aoba::Ops::Scale(mesh, extrudeResult.verts,
                     Aoba::Math::Vec3(0, yCent, zCent),
                     Aoba::Math::Vec3(scale.at(i), 1, scale.at(i)));
  }
  Aoba::Ops::MakeFace(mesh, last);

  // wings
  circle = Aoba::Ops::CreateCircle(mesh, 24, 6.75);
  Aoba::Ops::Scale(mesh, circle.verts, Aoba::Math::Vec3(0, 0, 0),
                   Aoba::Math::Vec3(0.09, 1, 1));
  Aoba::Ops::Rotate(
      mesh, circle.verts, Aoba::Math::Vec3(0, 0, 0),
      Aoba::Math::Mat3::Rotation(Aoba::Math::Vec3(0, 1, 0), 3.1415926 / 2));
  Aoba::Ops::Translate(mesh, circle.verts, Aoba::Math::Vec3(0, 0, -1.8));
  auto wingVerts = circle.verts;

  auto extrudeResult = Aoba::Ops::ExtrudeEdges(mesh, circle.edges);
  Aoba::Ops::Translate(mesh, extrudeResult.verts, Aoba::Math::Vec3(3.5, 0, 0));
  std::vector<Aoba::Core::Face*> facesToDuplicate = extrudeResult.faces;
  wingVerts.insert(wingVerts.end(), extrudeResult.verts.begin(),
                   extrudeResult.verts.end());

  extrudeResult = Aoba::Ops::ExtrudeEdges(mesh, extrudeResult.horizontalEdges);
  Aoba::Ops::Scale(mesh, extrudeResult.verts, Aoba::Math::Vec3(3.5, 0, -1.8),
                   Aoba::Math::Vec3(0.15, 0.15, 0.15));
  Aoba::Ops::Translate(mesh, extrudeResult.verts, Aoba::Math::Vec3(25, -12, 0));
  facesToDuplicate.insert(facesToDuplicate.end(), extrudeResult.faces.begin(),
                          extrudeResult.faces.end());
  wingVerts.insert(wingVerts.end(), extrudeResult.verts.begin(),
                   extrudeResult.verts.end());

  // add holding edge for subdivision
  extrudeResult = Aoba::Ops::ExtrudeEdges(mesh, extrudeResult.horizontalEdges);
  facesToDuplicate.insert(facesToDuplicate.end(), extrudeResult.faces.begin(),
                          extrudeResult.faces.end());
  wingVerts.insert(wingVerts.end(), extrudeResult.verts.begin(),
                   extrudeResult.verts.end());

  auto face = Aoba::Ops::MakeFace(mesh, extrudeResult.horizontalEdges);
  facesToDuplicate.push_back(face);

  // rotate wing, duplicate
  Aoba::Ops::Rotate(
      mesh, wingVerts, Aoba::Math::Vec3(0, 0, 0),
      Aoba::Math::Mat3::Rotation(Aoba::Math::Vec3(0, 1, 0), -0.05));
  auto duplicateResult =
      Aoba::Ops::Duplicate(mesh, std::vector<Aoba::Core::Vert*>(),
                           std::vector<Aoba::Core::Edge*>(), facesToDuplicate);
  Aoba::Ops::Scale(mesh, duplicateResult.verts, Aoba::Math::Vec3(0, 1, 0),
                   Aoba::Math::Vec3(-1, 1, 1));



  // tail and elevators? i think
  circle = Aoba::Ops::CreateCircle(mesh, 24, 4.15);
  Aoba::Ops::Scale(mesh, circle.verts, Aoba::Math::Vec3(0, 0, 0),
                   Aoba::Math::Vec3(0.12, 1, 1));
  wingVerts = circle.verts;

  extrudeResult = Aoba::Ops::ExtrudeEdges(mesh, circle.edges);
  Aoba::Ops::Scale(mesh, extrudeResult.verts, Aoba::Math::Vec3(0, 0, 0),
                   Aoba::Math::Vec3(0.3, 0.3, 0.3));
  Aoba::Ops::Translate(mesh, extrudeResult.verts,
                       Aoba::Math::Vec3(0, -6.6, 11));
  wingVerts.insert(wingVerts.end(), extrudeResult.verts.begin(),
                   extrudeResult.verts.end());
  facesToDuplicate = extrudeResult.faces;

  extrudeResult = Aoba::Ops::ExtrudeEdges(mesh, circle.edges);
  wingVerts.insert(wingVerts.end(), extrudeResult.verts.begin(),
                   extrudeResult.verts.end());
  facesToDuplicate.insert(facesToDuplicate.end(), extrudeResult.faces.begin(),
                          extrudeResult.faces.end());
  face = Aoba::Ops::MakeFace(mesh, extrudeResult.horizontalEdges);
  facesToDuplicate.push_back(face);

  // duplicate tail to make elevators
  duplicateResult =
      Aoba::Ops::Duplicate(mesh, std::vector<Aoba::Core::Vert*>(),
                           std::vector<Aoba::Core::Edge*>(), facesToDuplicate);
  Aoba::Ops::Rotate(mesh, duplicateResult.verts, Aoba::Math::Vec3(0, 0, 0),
                    Aoba::Math::Mat3::Rotation(Aoba::Math::Vec3(0, 1, 0), 1.5));
  wingVerts.insert(wingVerts.end(), duplicateResult.verts.begin(),
                   duplicateResult.verts.end());

  duplicateResult =
      Aoba::Ops::Duplicate(mesh, std::vector<Aoba::Core::Vert*>(),
                           std::vector<Aoba::Core::Edge*>(), facesToDuplicate);
  Aoba::Ops::Rotate(mesh, duplicateResult.verts, Aoba::Math::Vec3(0, 0, 0),
                    Aoba::Math::Mat3::Rotation(Aoba::Math::Vec3(0, 1, 0), -1.5));
  wingVerts.insert(wingVerts.end(), duplicateResult.verts.begin(),
                   duplicateResult.verts.end());

  Aoba::Ops::Translate(mesh, wingVerts, Aoba::Math::Vec3(0, -26.5, 1.2));


  // TODO: might do engines as well but you get the point


  Aoba::IO::ExportObj("aircraft.obj", mesh);

  return 0;
}