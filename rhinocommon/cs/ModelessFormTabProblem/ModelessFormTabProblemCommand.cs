using System;
using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace ModelessFormTabProblem
{
  [System.Runtime.InteropServices.Guid("32d03e4e-c9e1-46dd-ba0c-8a570a7255e1")]
  public class ModelessFormTabProblemCommand : Command
  {
    public ModelessFormTabProblemCommand()
    {
      // Rhino only creates one instance of each command class defined in a
      // plug-in, so it is safe to store a refence in a static property.
      Instance = this;
    }

    ///<summary>The only instance of this command.</summary>
    public static ModelessFormTabProblemCommand Instance
    {
      get;
      private set;
    }

    ///<returns>The command name as it appears on the Rhino command line.</returns>
    public override string EnglishName
    {
      get { return "ModelessFormTabProblemCommand"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var form = new Form1();
      var type = form.GetType();
      form.Show(RhinoApp.MainWindow());
      return Result.Success;
    }
  }
}
