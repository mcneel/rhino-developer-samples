using System;
using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleRhino
{
  [
    System.Runtime.InteropServices.Guid("efda8c97-1440-47f6-aa33-c024a7139d6a"), 
    CommandStyle(Style.Hidden)
  ]
  public class SampleRhinoCommand : Command
  {
    public SampleRhinoCommand()
    {
      // Rhino only creates one instance of each command class defined in a
      // plug-in, so it is safe to store a reference in a static property.
      Instance = this;
    }

    /// <summary>
    /// The only instance of this command.
    /// </summary>
    public static SampleRhinoCommand Instance
    {
      get;
      private set;
    }

    /// <summary>
    /// The command name as it appears on the Rhino command line.
    /// </summary>
    public override string EnglishName
    {
      get { return "SampleRhinoCommand"; }
    }

    /// <summary>
    /// Rhino calls this function to run the command.
    /// </summary>
    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      RhinoApp.WriteLine("{0} loaded.", SampleRhinoPlugIn.Instance.Name);
      return Result.Success;
    }
  }
}
