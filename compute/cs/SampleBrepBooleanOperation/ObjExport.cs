using System;
using System.IO;
using System.Collections.Generic;
using Rhino.Geometry;

namespace Rhino.Compute
{
    public static class ObjExport
    {
        public static void ExportBrepsToObj(string filePath, IEnumerable<Brep> breps)
        {
            List<Mesh> meshes = new List<Mesh>();
            foreach (Brep brep in breps)
            {
                Mesh[] brepMeshes = MeshCompute.CreateFromBrep(brep);
                foreach (Mesh brepMesh in brepMeshes)
                {
                    meshes.Add(brepMesh);
                }
            }
            ExportMeshesToObj(filePath, meshes);
        }

        public static void ExportMeshesToObj(string filePath, IEnumerable<Mesh> meshes)
        {
            using (StreamWriter file = new StreamWriter(filePath, false))
            {
                int meshIndex = 0;
                int vertexOffset = 1;
                foreach (Mesh mesh in meshes)
                {
                    if (mesh.Vertices.Count != mesh.Normals.Count)
                    {
                        throw new System.Exception("Different number of vertices and normals in the mesh.");
                    }

                    file.WriteLine("g Mesh-{0}", meshIndex);
                    foreach (Point3f vertex in mesh.Vertices)
                    {
                        file.WriteLine("v {0} {1} {2}", vertex.X, vertex.Y, vertex.Z);
                    }
                    foreach (Vector3f normal in mesh.Normals)
                    {
                        file.WriteLine("vn {0} {1} {2}", normal.X, normal.Y, normal.Z);
                    }
                    foreach (MeshFace face in mesh.Faces)
                    {
                        if (face.IsTriangle)
                        {
                            file.WriteLine("f {0}//{1} {2}//{3} {4}//{5}",
                                vertexOffset + face.A, vertexOffset + face.A,
                                vertexOffset + face.B, vertexOffset + face.B,
                                vertexOffset + face.C, vertexOffset + face.C
                            );
                        }
                        else if (face.IsQuad)
                        {
                            file.WriteLine("f {0}//{1} {2}//{3} {4}//{5} {6}//{7}",
                                vertexOffset + face.A, vertexOffset + face.A,
                                vertexOffset + face.B, vertexOffset + face.B,
                                vertexOffset + face.C, vertexOffset + face.C,
                                vertexOffset + face.D, vertexOffset + face.D
                            );
                        }
                    }

                    meshIndex += 1;
                    vertexOffset += mesh.Vertices.Count;
                }
            }
        }
    }
}
