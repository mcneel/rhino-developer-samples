using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;
using Rhino.Render;
using Rhino;

namespace SampleCsCommands
{
  /// <summary>
  /// This command demonstrates how to set up cached texture coordinates based on the material texture channels.
  /// </summary>
  public class SampleCsCachedTextureCoordinates : Command
  {
    public override string EnglishName => "SampleCsCachedTextureCoordinates";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select objects");
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      foreach (var rhinoObjectRef in go.Objects())
      {
        RhinoObject rhinoObject = rhinoObjectRef.Object();
        if (null == rhinoObject)
          continue;

        // Get the object material, this material is used where subobject material is not defined
        var objectMaterial = rhinoObject.GetMaterial(true);

        // Get the render meshes from the object
        var meshes = rhinoObject.GetMeshes(MeshType.Render);

        // If there are no render meshes, create them and get them again
        if (meshes == null || meshes.Length == 0)
        {
          rhinoObject.CreateMeshes(MeshType.Render, rhinoObject.GetRenderMeshParameters(), false);
          meshes = rhinoObject.GetMeshes(MeshType.Render);
          if (meshes == null || meshes.Length == 0)
          {
            continue;
          }
        }

        RhinoApp.WriteLine($"Object {rhinoObject.Id} with {meshes.Length} meshes");

        // Iterate through the meshes, each polysurface face has a corresponding mesh
        for (int mi = 0; mi < meshes.Length; mi++)
        {
          var mesh = meshes[mi];
          if (null == mesh)
            continue;

          // Figure out which material to use for this mesh
          var meshMaterial = objectMaterial;
          if (rhinoObject.HasSubobjectMaterials)
          {
            // If this object has subobject materials, figure out what is the component type of its subobject
            ComponentIndexType ciSubMaterialComponentType = ComponentIndexType.InvalidType;
            if (rhinoObject.ObjectType == ObjectType.SubD)
              ciSubMaterialComponentType = ComponentIndexType.SubdFace;
            else if (rhinoObject.ObjectType == ObjectType.Brep)
              ciSubMaterialComponentType = ComponentIndexType.BrepFace;
            else if (rhinoObject.ObjectType == ObjectType.Extrusion)
              ciSubMaterialComponentType = ComponentIndexType.BrepFace;

            // Ask if there is a subobject material for the current subobject
            ComponentIndex ci = new ComponentIndex(ciSubMaterialComponentType, mi);
            var subObjectMaterial = rhinoObject.GetMaterial(ci);
            if (subObjectMaterial != null)
              meshMaterial = subObjectMaterial;
          }

          RhinoApp.WriteLine($"  Mesh {mi} material {meshMaterial.Id}");

          // Get all the textures used by the material
          var textures = meshMaterial.GetTextures();

          // Set up cached texture coordinates for each texture
          foreach (var texture in textures)
          {
            if (texture == null)
              continue;

            // Get texture mapping based on the texture mapping channel id. In case of WCS projected texture no mapping is returned.
            TextureMapping tm = rhinoObject.GetTextureMapping(texture.MappingChannelId, out Transform objectTransform);
            if (tm != null && tm.IsValid)
            {
              mesh.SetCachedTextureCoordinates(tm, ref objectTransform);
            }
          }

          // Get cached texture coordinates for each texture
          foreach (var texture in textures)
          {
            if (texture == null)
              continue;

            // Get texture mapping based on the texture mapping channel id. In case of WCS projected texture no mapping is returned.
            TextureMapping tm = rhinoObject.GetTextureMapping(texture.MappingChannelId, out Transform objectTransform);
            if (tm != null && tm.IsValid)
            {
              // Get the cached texture coordinates for this texture
              CachedTextureCoordinates cachedTCs = mesh.GetCachedTextureCoordinates(tm.Id);
              if (cachedTCs != null && cachedTCs.Count == mesh.Vertices.Count)
              {
                RhinoApp.Write($"    Cached texture coordinates for {texture.TextureType} texture. ");
                RhinoApp.Write($"Mapping channel {texture.MappingChannelId} used.");
                RhinoApp.Write($"\n");
              }
            }
          }
        }
      }
      return Result.Success;
    }
  }
}