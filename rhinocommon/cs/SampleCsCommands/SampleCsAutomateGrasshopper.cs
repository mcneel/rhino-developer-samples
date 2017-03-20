using System;
using System.IO;
using System.Reflection;
using System.Windows.Forms;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("cb825773-b04f-49df-9c12-5e9134696854")]
  public class SampleCsAutomateGrasshopper : Command
  {
    /// <summary>
    /// Grasshopper's plug-in Guid
    /// </summary>
    private readonly Guid m_gh_guid = new Guid("b45a29b1-4343-4035-989e-044e8580d9cf");

    /// <summary>
    /// Constructor
    /// </summary>
    public SampleCsAutomateGrasshopper()
    {
    }

    /// <summary>
    /// EnglishName override
    /// </summary>
    public override string EnglishName
    {
      get { return "SampleCsAutomateGrasshopper"; }
    }

    /// <summary>
    /// RunCommand override
    /// </summary>
    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Verify that Grasshopper is installed
      string gh_path = Rhino.PlugIns.PlugIn.PathFromId(m_gh_guid);
      if (string.IsNullOrEmpty(gh_path) && File.Exists(gh_path))
      {
        RhinoApp.WriteLine("Grasshopper not installed.");
        return Result.Cancel;
      }

      // Prompt the user for a Grasshopper solution to open
      string filename = string.Empty;
      if (mode == RunMode.Interactive)
      {
        OpenFileDialog dialog = new OpenFileDialog();
        dialog.Filter = @"Grasshopper Files (*.gh;*.ghx)|*.gh; *.ghx||";
        dialog.DefaultExt = "gh";
        DialogResult rc = dialog.ShowDialog();
        if (rc != DialogResult.OK)
          return Result.Cancel;

        filename = dialog.FileName;
      }
      else
      {
        Result rc = Rhino.Input.RhinoGet.GetString("Grasshopper file to open", false, ref filename);
        if (rc != Result.Success)
          return rc;
      }

      // Verify Grasshopper file
      if (string.IsNullOrEmpty(filename) || !File.Exists(filename))
        return Result.Failure;

      try
      {
        // Try getting the Grasshopper COM object
        object gh_object = RhinoApp.GetPlugInObject(m_gh_guid);

        // Create a parameters array
        object[] parameters = new object[1];
        parameters[0] = filename;

        // Invoke the OpenDocument method
        gh_object.GetType().InvokeMember("OpenDocument", BindingFlags.InvokeMethod, null, gh_object, parameters);

        // You can use the folloiwng to dump the names and types of
        // all of the members to the command line...

        //foreach (MemberInfo member in gh_object.GetType().GetMembers())
        //  RhinoApp.WriteLine("{0} -- {1}", member.MemberType, member);
      }
      catch (Exception ex)
      {
        RhinoApp.WriteLine(ex.Message);
      }

      return Result.Success;
    }
  }

}
