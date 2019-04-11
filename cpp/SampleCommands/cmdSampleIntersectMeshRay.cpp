#include "stdafx.h"

/*
Description:
  Finds the first intersection of a ray with a mesh.
Parameters:
  mesh         - [in]   - A mesh to intersect.
  ray          - [in]   - A ray to be casted.
  face_indices - [out]  - Faces on mesh that ray intersects (can be nullptr).
Returns:
  >= 0.0 parameter along ray if successful. < 0.0 if no intersection found.
Remarks:
  The ray may intersect more than one face in cases where the ray hits the edge
  between two faces or the vertex corner shared by multiple faces.
*/
double SampleIntersectMeshRay(const ON_Mesh* mesh, ON_3dRay* ray, ON_SimpleArray<int>* face_indices)
{
  double rc = -1.0;
  if (mesh && ray)
  {
    ON_3dVector dir = ray->m_V;
    const ON_MeshTree* mesh_tree = mesh->MeshTree(true);
    if (mesh_tree && dir.Unitize())
    {
      // increase the range by a factor of 2 so we are confident the
      // line passes entirely through the mesh
      double range = mesh_tree->m_bbox.MaximumDistanceTo(ray->m_P) * 2.0;
      ON_Line line(ray->m_P, ray->m_P + range * dir);

      ON_SimpleArray<ON_CMX_EVENT> hits;
      mesh_tree->IntersectLine(line, hits);
      int hit_count = hits.Count();
      if (hit_count > 0)
      {
        ON_SimpleArray<double> tvals;
        ON_SimpleArray<int> indices;
        // tmin should be between 0 and 1 for the line
        double tmin = 100.0;
        for (int i = 0; i < hit_count; i++)
        {
          const ON_CMX_EVENT& e = hits[i];
          if (e.m_C[0].m_t <= tmin)
          {
            tmin = e.m_C[0].m_t;
            if (face_indices)
            {
              tvals.Append(tmin);
              indices.Append(e.m_M[0].m_face_index);
            }
          }
          if (e.m_type == ON_CMX_EVENT::cmx_overlap && e.m_C[1].m_t <= tmin)
          {
            tmin = e.m_C[1].m_t;
            if (face_indices)
            {
              tvals.Append(tmin);
              indices.Append(e.m_M[1].m_face_index);
            }
          }
        }
        if (tmin >= 0 && tmin <= 1.0)
        {
          if (face_indices)
          {
            for (int i = 0; i < tvals.Count(); i++)
            {
              if (tvals[i] == tmin)
                face_indices->Append(indices[i]);
            }
          }

          double line_length = line.Length();
          double ray_length = ray->m_V.Length();
          if (ray_length > ON_SQRT_EPSILON)
          {
            rc = tmin * line_length / ray_length;
          }
        }
      }
    }
  }
  return rc;
}