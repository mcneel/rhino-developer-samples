using System;
using System.Collections.Generic;
using System.IO;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace Test3dmWrite
{
    public class Test3dmWriteCommand : Command
    {
        public Test3dmWriteCommand()
        {
            // Rhino only creates one instance of each command class defined in a
            // plug-in, so it is safe to store a refence in a static property.
            Instance = this;
        }

        ///<summary>The only instance of this command.</summary>
        public static Test3dmWriteCommand Instance { get; private set; }

        ///<returns>The command name as it appears on the Rhino command line.</returns>
        public override string EnglishName => "Test3dmWriteCommand";

        protected override Result RunCommand(RhinoDoc doc, RunMode mode)
        {
            var docH = RhinoDoc.CreateHeadless(null);
            var file3dm = new Rhino.FileIO.File3dm();
            var mesh = new Rhino.Geometry.Mesh();

            mesh.Vertices.Add(0.0, 0.0, 10.0); //0
            mesh.Vertices.Add(1.0, 0.0, 1.0); //1
            mesh.Vertices.Add(2.0, 0.0, 1.0); //2
            mesh.Vertices.Add(3.0, 0.0, 0.0); //3
            mesh.Vertices.Add(0.0, 1.0, 1.0); //4
            mesh.Vertices.Add(1.0, 1.0, 2.0); //5
            mesh.Vertices.Add(2.0, 1.0, 1.0); //6
            mesh.Vertices.Add(3.0, 1.0, 0.0); //7
            mesh.Vertices.Add(0.0, 2.0, 1.0); //8
            mesh.Vertices.Add(1.0, 2.0, 1.0); //9
            mesh.Vertices.Add(2.0, 2.0, 1.0); //10
            mesh.Vertices.Add(3.0, 2.0, 1.0); //11

            mesh.Faces.AddFace(0, 1, 5, 4);
            mesh.Faces.AddFace(1, 2, 6, 5);
            mesh.Faces.AddFace(2, 3, 7, 6);
            mesh.Faces.AddFace(4, 5, 9, 8);
            mesh.Faces.AddFace(5, 6, 10, 9);
            mesh.Faces.AddFace(6, 7, 11, 10);
            mesh.Normals.ComputeNormals();
            mesh.Compact();

            Console.WriteLine($"PartitionCount: {mesh.PartitionCount}");

            docH.Objects.AddMesh(mesh);
            file3dm.Objects.AddMesh(mesh);

            var path = Path.GetTempPath();
            var pDoc = Path.Combine(path, "doc77.3dm");
            var p3dm = Path.Combine(path, "file77.3dm");

            Console.WriteLine(pDoc);
            Console.WriteLine(p3dm);

            docH.Write3dmFile(pDoc, new Rhino.FileIO.FileWriteOptions());
            file3dm.Write(p3dm, 8);

            // ---
            return Result.Success;
        }
    }
}

