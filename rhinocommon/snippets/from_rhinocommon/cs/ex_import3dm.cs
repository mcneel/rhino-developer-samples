namespace examples_cs
{
  [
    System.Runtime.InteropServices.Guid("7e94aa7f-93a8-4d91-aabd-5560e78b4b83"),
    // Required for commands that call Rhino.RhinoApp.RunScript
    Rhino.Commands.CommandStyle(Rhino.Commands.Style.ScriptRunner)
  ]
  public class ex_import3dm : Rhino.Commands.Command
  {
    public ex_import3dm()
    {
    }

    public override string EnglishName
    {
      get { return "cs_import3dm"; }
    }

    protected override Rhino.Commands.Result RunCommand(Rhino.RhinoDoc doc, Rhino.Commands.RunMode mode)
    {
      string filename = string.Empty;
      if (mode == Rhino.Commands.RunMode.Interactive)
        filename = Rhino.Input.RhinoGet.GetFileName(Rhino.Input.Custom.GetFileNameMode.OpenRhinoOnly, null, "Import", Rhino.RhinoApp.MainWindow());
      else
        Rhino.Input.RhinoGet.GetString("Name of Rhino file to import", false, ref filename);

      filename = filename.Trim();
      if (string.IsNullOrEmpty(filename))
        return Rhino.Commands.Result.Cancel;

      if (!System.IO.File.Exists(filename))
      {
        Rhino.RhinoApp.WriteLine("File not found.");
        return Rhino.Commands.Result.Failure;
      }

      // Make sure to surround filename string with double-quote characters
      // in case the path contains spaces.
      string script = string.Format("_-Import \"{0}\" _Enter", filename);
      Rhino.RhinoApp.RunScript(script, false);
      
      return Rhino.Commands.Result.Success;
    }
  }
}
