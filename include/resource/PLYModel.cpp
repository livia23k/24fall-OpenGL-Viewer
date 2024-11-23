#include "PLYModel.h"

PLYModel::PLYModel()
{
    is_triangulated = false;

    vertices.clear();
    faces.clear();
};

void PLYModel::Triangulate()
{
    if (is_triangulated)
    {
        return;
    }

    std::vector<Face> tri_faces;

    for (const Face &face : faces)
    {
        if (face.indices.size() == 3)
        {
            tri_faces.push_back(face);
        }
        else
        {
            int32_t tri_count = face.indices.size() - 2;
            for (int32_t i = 0; i < tri_count; ++i)
            {
                Face tri{
                    .indices_count = 3,
                    .indices = {face.indices[0], face.indices[i + 1], face.indices[i + 2]}};

                tri_faces.push_back(tri);
            }
        }
    }

    faces = tri_faces;
    is_triangulated = true;
}
