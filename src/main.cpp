#include "AobaAPI/AobaAPI.hpp"

int main() {
  Aoba::Core::Mesh* mesh = new Aoba::Core::Mesh();

  auto circle = Aoba::Ops::CreateCircle(mesh, 16, 1);
  auto bottomFace = Aoba::Ops::MakeFace(mesh, circle.edges);
  auto extrudeResult = Aoba::Ops::ExtrudeEdges(mesh, circle.edges);
  Aoba::Ops::Translate(mesh, extrudeResult.verts, Aoba::Math::Vec3(0, 0, 3));
  auto topFace = Aoba::Ops::MakeFace(mesh, extrudeResult.horizontalEdges);

  auto verts = mesh->Verts();
  auto edges = mesh->Edges();
  auto faces = mesh->Faces();

  auto duplicateResult =
      Aoba::Ops::Duplicate(mesh, mesh->Verts(), mesh->Edges(), mesh->Faces());

  Aoba::Ops::Rotate(
      mesh, duplicateResult.verts, Aoba::Math::Vec3(1, 0, 1.5),
      Aoba::Math::Mat3::Rotation(Aoba::Math::Vec3(0, 1, 0), 1.25));

  Aoba::Ops::Scale(mesh, duplicateResult.verts, Aoba::Math::Vec3(1, 1, 1),
                   Aoba::Math::Vec3(1, 2, 0.5));

  verts = mesh->Verts();
  edges = mesh->Edges();
  faces = mesh->Faces();

  Aoba::IO::ExportObj("testObject.obj", mesh);

  return 0;
}