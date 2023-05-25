using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input.Custom;

namespace SampleCsUserData.Commands
{
  public class SampleCsAddBrepFaceUserData : Command
  {
    public override string EnglishName => "SampleCsAddBrepFaceUserData";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Allow for picking of either a surface or a brep face
      var go = new GetObject
      {
        GeometryFilter = ObjectType.Surface,
        SubObjectSelect = true
      };
      go.SetCommandPrompt("Select surface to attach data");
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      // Get first object reference. Note, this reference represents the picked
      // surface or face, not the top level brep.
      var obj_ref = go.Object(0);

      // Get brep face that was picked
      var face = obj_ref.Face();
      if (null == face)
        return Result.Failure;

      // Since the object reference only represents the picked surface or face, 
      // we need to get the owning brep from the Rhino object.
      if (!(obj_ref.Object() is BrepObject brep_object))
        return Result.Failure;

      // Get the brep object's brep geometry
      var brep = brep_object.BrepGeometry;
      if (null == brep)
        return Result.Failure;

      // Get the brep face's underlying surface
      var surface_index = face.SurfaceIndex;
      var srf = brep.Surfaces[surface_index];
      if (null == srf)
        return Result.Failure;

      // Query the surface for user data
      if (srf.UserData.Find(typeof(SampleCsUserDataObject)) is SampleCsUserDataObject ud)
      {
        RhinoApp.WriteLine("{0} = {1}", ud.Description, ud.Notes);
        return Result.Success;
      }

      // Prompt for a string
      var gs = new GetString();
      gs.SetCommandPrompt("Surface notes");
      gs.GetLiteralString();
      if (gs.CommandResult() != Result.Success)
        return gs.CommandResult();

      // Duplicate the brep
      var new_brep = brep.DuplicateBrep();
      if (null != new_brep)
      {
        // Get the brep face's underlying surface
        srf = new_brep.Surfaces[surface_index];
        if (null != srf)
        {
          // New up the user data
          ud = new SampleCsUserDataObject
          {
            Notes = gs.StringResult()
          };

          // Attach the user data to the surface
          srf.UserData.Add(ud);

          // Replace object. Note, we cannot reuse obj_ref because it only references
          // the picked surface or face, not the top level brep. So, just pass the
          // Rhino object's id instead.
          doc.Objects.Replace(brep_object.Id, new_brep);
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
