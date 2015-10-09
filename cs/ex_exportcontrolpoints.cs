using System;

partial class Examples
{
  public static Rhino.Commands.Result ExportControlPoints(Rhino.RhinoDoc doc)
  {
    Rhino.DocObjects.ObjRef objref;
    var get_rc = Rhino.Input.RhinoGet.GetOneObject("Select curve", false, Rhino.DocObjects.ObjectType.Curve, out objref);
    if (get_rc != Rhino.Commands.Result.Success)
      return get_rc;
    var curve = objref.Curve();
    if (curve == null)
      return Rhino.Commands.Result.Failure;
    var nc = curve.ToNurbsCurve();

    var fd = new System.Windows.Forms.SaveFileDialog();
    fd.Filter = "Text Files | *.txt";
    fd.DefaultExt = "txt";
    if( fd.ShowDialog(Rhino.RhinoApp.MainWindow())!= System.Windows.Forms.DialogResult.OK)
      return Rhino.Commands.Result.Cancel;
    string path = fd.FileName;
    using( System.IO.StreamWriter sw = new System.IO.StreamWriter(path) )
    {
      foreach( var pt in nc.Points )
      {
        var loc = pt.Location;
        sw.WriteLine("{0} {1} {2}", loc.X, loc.Y, loc.Z);
      }
      sw.Close();
    }
    return Rhino.Commands.Result.Success;
  }
}
