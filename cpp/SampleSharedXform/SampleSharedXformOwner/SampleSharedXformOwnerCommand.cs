using System;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleSharedXformOwner
{
  public class SampleSharedXformOwnerCommand : Command
  {
    /// <summary>
    /// Command constructor
    /// </summary>
    public SampleSharedXformOwnerCommand()
    {
      Rnd = new Random(Guid.NewGuid().GetHashCode());
      RhinoDoc.CloseDocument += OnCloseDocument;
    }

    /// <summary>
    /// RhinoDoc.CloseDocument event handler
    /// </summary>
    public static void OnCloseDocument(object sender, DocumentEventArgs e)
    {
      // When the document closes, empty the transformations table.
      SampleSharedXformTable.Empty();
    }

    /// <summary>
    /// The command's English name
    /// </summary>
    public override string EnglishName => "SampleSharedXformOwner";

    /// <summary>
    /// A useful random number generator
    /// </summary>
    public Random Rnd { get; }

    /// <summary>
    /// Called by Rhino when the user wants to run this command.
    /// </summary>
    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      ReportSampleSharedXformTable("Before transform table operations:");

      var go = new GetObject();
      go.SetCommandPrompt("Select objects to add random transformations");
      go.SubObjectSelect = false;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      foreach (var obj_ref in go.Objects())
      {
        var count = Rnd.Next(50, 100);
        var transforms = new Transform[count];
        for (var i = 0; i < count; i++)
        {
          var degrees = Rnd.Next(0, 360);
          var radians = RhinoMath.ToRadians(degrees);
          transforms[i] = Transform.Rotation(radians, Point3d.Origin);
        }

        if (SampleSharedXformTable.Contains(obj_ref.ObjectId))
          SampleSharedXformTable.Update(obj_ref.ObjectId, transforms);
        else
          SampleSharedXformTable.Add(obj_ref.ObjectId, transforms);
      }

      ReportSampleSharedXformTable("After transform table operations:");

      return Result.Success;
    }

    /// <summary>
    /// Dump some table into to the command line
    /// </summary>
    private void ReportSampleSharedXformTable(string message)
    {
      RhinoApp.WriteLine(message);
      RhinoApp.WriteLine("  SampleSharedXformTable.Count: {0}", SampleSharedXformTable.Count());
      var ids = SampleSharedXformTable.Ids();
      foreach (var id in ids)
      {
        var transforms = SampleSharedXformTable.Transforms(id);
        RhinoApp.WriteLine("  Object id: {0}, transform count: {1}", id.ToString(), transforms.Length.ToString());
      }
    }
  }
}
