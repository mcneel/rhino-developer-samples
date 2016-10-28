partial class Examples
{
  public static Rhino.Commands.Result HatchCurve(Rhino.RhinoDoc doc)
  {
    var go = new Rhino.Input.Custom.GetObject();
    go.SetCommandPrompt("Select closed planar curve");
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve;
    go.GeometryAttributeFilter = Rhino.Input.Custom.GeometryAttributeFilter.ClosedCurve;
    go.SubObjectSelect = false;
    go.Get();
    if( go.CommandResult() != Rhino.Commands.Result.Success )
      return go.CommandResult();

    var curve = go.Object(0).Curve();
    if( curve==null || !curve.IsClosed || !curve.IsPlanar() )
      return Rhino.Commands.Result.Failure;

    string hatch_name = doc.HatchPatterns[doc.HatchPatterns.CurrentHatchPatternIndex].Name;
    var rc = Rhino.Input.RhinoGet.GetString("Hatch pattern", true, ref hatch_name);
    if( rc!= Rhino.Commands.Result.Success )
      return rc;
    hatch_name = hatch_name.Trim();
    if( string.IsNullOrWhiteSpace(hatch_name) )
      return Rhino.Commands.Result.Nothing;
    int index = doc.HatchPatterns.Find(hatch_name, true);
    if( index < 0 )
    {
      Rhino.RhinoApp.WriteLine("Hatch pattern does not exist.");
      return Rhino.Commands.Result.Nothing;
    }

    var hatches = Rhino.Geometry.Hatch.Create( curve, index, 0, 1);
    for( int i=0; i<hatches.Length; i++ )
      doc.Objects.AddHatch(hatches[i]);
    if( hatches.Length>0 )
      doc.Views.Redraw();
    return Rhino.Commands.Result.Success;
  }
}
