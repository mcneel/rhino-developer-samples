using System;
using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;
using Rhino.DocObjects;
using Rhino.Render;

namespace SampleCsCustomMeshMapping
{
  public class SampleCsCustomMeshMappingCommand : Command
  {
    public SampleCsCustomMeshMappingCommand()
    {
      // Rhino only creates one instance of each command class defined in a
      // plug-in, so it is safe to store a refence in a static property.
      Instance = this;
    }

    ///<summary>The only instance of this command.</summary>
    public static SampleCsCustomMeshMappingCommand Instance
    {
      get; private set;
    }

    ///<returns>The command name as it appears on the Rhino command line.</returns>
    public override string EnglishName
    {
      get { return "SampleCsCustomMeshMappingCommand"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Select object

      ObjRef objRef = null;
      using (GetObject go = new GetObject())
      {
        go.SetCommandPrompt("Select object to apply custom mesh mapping to");
        go.GeometryFilter = ObjectType.Brep | ObjectType.Extrusion | ObjectType.Mesh;
        var goRes = go.Get();
        if (goRes != GetResult.Object)
        {
          return go.CommandResult();
        }
        objRef = go.Object(0);
      }

      if (doc.Objects.FindId(objRef.ObjectId) is RhinoObject rhinoObject)
      {
        // Get render meshes from the object

        var renderMeshes = rhinoObject.GetMeshes(Rhino.Geometry.MeshType.Render);
        if (renderMeshes != null && renderMeshes.Length > 0 && renderMeshes[0] != null)
        {
          // Create mapping mesh from render meshes

          Mesh mappingMesh = new Mesh();
          foreach (var renderMesh in renderMeshes)
          {
            if (renderMesh != null)
            {
              mappingMesh.Append(renderMesh);
            }
          }
          for (int vi = 0; vi < mappingMesh.Vertices.Count; vi++)
          {
            Point2f tc = new Point2f(mappingMesh.Vertices[vi].X, mappingMesh.Vertices[vi].Y);
            mappingMesh.TextureCoordinates.SetTextureCoordinate(vi, tc);
          }

          // Create custom mesh texture mapping

          TextureMapping textureMapping = TextureMapping.CreateCustomMeshMapping(mappingMesh);

          // Apply mapping to object

          rhinoObject.SetTextureMapping(1, textureMapping);
        }
      }
      return Result.Success;
    }
  }
}
