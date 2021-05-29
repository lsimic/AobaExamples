#include "AobaAPI/AobaAPI.hpp"
#include <random>

int main() {
  std::default_random_engine generator(10);
  std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

  // crate a new mesh
  Aoba::Core::Mesh* mesh = new Aoba::Core::Mesh();

  // Start with a circle
  auto circle = Aoba::Ops::CreateCircle(mesh, 8, 0.5f);

  // extrude circle and scale inwards, then extrude upwards to create a torus
  auto eeResult = Aoba::Ops::ExtrudeEdges(mesh, mesh->Edges());
  Aoba::Ops::Scale(mesh, eeResult.verts, Aoba::Math::Vec3(0, 0, 0),
                   Aoba::Math::Vec3(3.0f, 3.0f, 1.0f));
  auto efrResult = Aoba::Ops::ExtrudeFaceRegion(mesh, mesh->Faces(), true);
  Aoba::Ops::Translate(mesh, efrResult.verts, Aoba::Math::Vec3(0, 0, 1.0f));
  Aoba::Ops::ReverseFaceOrder(mesh, efrResult.horizontalFaces);
  Aoba::Ops::ReverseFaceOrder(mesh, efrResult.verticalFaces);

  // subdivide the geometry twice to create a smoother torus shape
  auto subdResult = Aoba::Ops::SubdivideSmooth(mesh, {}, mesh->Faces());
  subdResult = Aoba::Ops::SubdivideSmooth(mesh, {}, mesh->Faces());

  // select all faces with average vertex coords > 0.5
  // if face is between 0.3-0.5, select it at random
  std::vector<Aoba::Core::Face*> frostingFaces;
  frostingFaces = mesh->Faces([](const Aoba::Core::Face* const f) {
    auto center = f->CalcCenterAverage();
    if (center.z > 0.5) {
      return true;
    }
    return false;
  });
  auto moreFaces = mesh->Faces([](const Aoba::Core::Face* const f) {
    auto center = f->CalcCenterAverage();
    if (center.z > 0.3f && center.z < 0.5f) {
      return true;
    }
    return false;
  });
  for (Aoba::Core::Face* f : moreFaces) {
    float rand = distribution(generator);
    if (rand > 0.5) {
      frostingFaces.push_back(f);
    }
  }

  // duplicate frosting faces and move them on face normals
  auto dupes = Aoba::Ops::Duplicate(mesh, {}, {}, frostingFaces);
  // mark faces used for calculation
  for (Aoba::Core::Face* f : dupes.faces) {
    f->NormalUpdate();
    f->flags = 1;
  }
  for (Aoba::Core::Vert* v : dupes.verts) {
    auto vertFaces =
        v->Faces([](const Aoba::Core::Face* const f) { return f->flags == 1; });
    auto avgNo = Aoba::Math::Vec3();
    for (Aoba::Core::Face* f : vertFaces) {
      avgNo += f->no;
    }
    avgNo.Normalize();
    avgNo.Negate();
    v->co += 0.05f * avgNo;
  }

  // find, extrude boundary edges,
  auto boundary = mesh->Edges(
      [](const Aoba::Core::Edge* const e) { return e->IsBoundary(); });
  auto extResult = Aoba::Ops::ExtrudeEdges(mesh, boundary);

  // move frosting verts out.
  for (Aoba::Core::Vert* v : dupes.verts) {
    auto vertFaces =
        v->Faces([](const Aoba::Core::Face* const f) { return f->flags == 1; });
    auto avgNo = Aoba::Math::Vec3(0, 0, 0);
    for (Aoba::Core::Face* f : vertFaces) {
      f->NormalUpdate();
      avgNo += f->no;
    }
    avgNo.Normalize();
    v->co += 0.1f * avgNo;
  }

  // subdivide more
  subdResult = Aoba::Ops::SubdivideSmooth(mesh, {}, mesh->Faces());
  subdResult = Aoba::Ops::SubdivideSmooth(mesh, {}, mesh->Faces());

  // add slight variation to each vertex, to make the donut less smooth
  for (Aoba::Core::Vert* v : mesh->Verts()) {
    auto avgNo = Aoba::Math::Vec3(0, 0, 0);
    for (Aoba::Core::Face* f : v->Faces()) {
      f->NormalUpdate();
      avgNo += f->no;
    }
    avgNo.Normalize();
    v->co += distribution(generator) * 0.005f * avgNo;
  }

  // add sprinkles
  auto sph = Aoba::Ops::CreateUVSphere(mesh, 0.02f, 4, 6);
  for (Aoba::Core::Face* f : mesh->Faces()) {
    f->NormalUpdate();
    if (f->no.z > 0.2f) {
      if (distribution(generator) > 0.8f) {
        auto dup = Aoba::Ops::Duplicate(mesh, {}, {}, sph.Faces);
        Aoba::Ops::Scale(mesh, dup.verts, Aoba::Math::Vec3(0, 0, 0),
                         Aoba::Math::Vec3(distribution(generator) * 2,
                                          distribution(generator) * 2,
                                          distribution(generator) * 2));
        Aoba::Ops::Translate(mesh, dup.verts, f->CalcCenterAverage());
      }
    }
  }
  Aoba::Ops::Delete(mesh, sph.verts, {}, {}, Aoba::Ops::DeleteMode::All);

  // export the mesh
  Aoba::IO::ExportObj("05_Donut.obj", mesh);

  return 0;
}
