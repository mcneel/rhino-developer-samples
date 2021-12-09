
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input;
using Rhino.Input.Custom;
using Rhino.Render;

// This sample will only work in Rhino 7.13 upwards because the RenderMaterial.AssignTo()
// function was only added to the Rhino SDK in 7.13.

namespace SampleCsCommands
{
  public class SampleCsSetBrepFaceMaterial : Command
  {
    public SampleCsSetBrepFaceMaterial() { }

    public override string EnglishName { get { return "SampleCsSetBrepFaceMaterial"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select a whole brep object to set face materials to");
      go.GeometryFilter = ObjectType.Brep;
      if (go.Get() != GetResult.Object)
        return go.CommandResult();

      // Get the selected object. We need it to be a brep for this example.
      var obj = go.Object(0).Object();
      if (obj is ExtrusionObject)
      {
        RhinoApp.WriteLine("The selected object is an extrusion. Try running Explode and then try this command again");
        return Result.Failure;
      }

      if (!(obj is BrepObject brepObject))
      {
        RhinoApp.WriteLine("The selected object is not a brep object");
        return Result.Failure;
      }

      // Create a red render material and attach it to the document.
      var red_rhino_material = new Material { DiffuseColor = System.Drawing.Color.Red };
      var red_render_material = RenderMaterial.CreateBasicMaterial(red_rhino_material, doc);
      doc.RenderMaterials.Add(red_render_material);

      // Create a blue render material and attach it to the document.
      var blue_rhino_material = new Material { DiffuseColor = System.Drawing.Color.Blue };
      var blue_render_material = RenderMaterial.CreateBasicMaterial(blue_rhino_material, doc);
      doc.RenderMaterials.Add(blue_render_material);

      // Change the object material to the blue material.
      using (var brepref = new ObjRef(doc, brepObject.Id))
      {
        blue_render_material.AssignTo(brepref);
      }

      //#######################################################################################
      //#######################################################################################
      //#######################################################################################
      // This doesn't work yet -- possibly because of https://mcneel.myjetbrains.com/youtrack/issue/RH-66614
      //#######################################################################################
      //#######################################################################################
      //#######################################################################################

      // Set every second face material to the red material.
      var brep = brepObject.BrepGeometry;
      for (int i = 0; i < brep.Faces.Count; i++)
      {
        var face = brep.Faces[i];

        if ((i % 2) == 0)
        {
          using (var faceref = new ObjRef(doc, brepObject.Id, face.ComponentIndex()))
          {
            red_render_material.AssignTo(faceref);
          }
        }
        else
        {
          face.ClearMaterialChannelIndex();
        }
      }

      // Commit the changes.
//      brepObject.CommitChanges();

      return Result.Success;
    }
  }
}
