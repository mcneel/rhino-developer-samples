using System;
using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace CustomSunSections
{
  public class CustomSunSectionsCommand : Command
  {
    public CustomSunSectionsCommand()
    {
      // Rhino only creates one instance of each command class defined in a
      // plug-in, so it is safe to store a refence in a static property.
      Instance = this;
    }

    ///<summary>The only instance of this command.</summary>
    public static CustomSunSectionsCommand Instance
    {
      get; private set;
    }

    ///<returns>The command name as it appears on the Rhino command line.</returns>
    public override string EnglishName
    {
      get { return "CustomSunSectionsCommand"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // TODO: start here modifying the behaviour of your command.
      // ---
      // ---

      return Result.Success;
    }
  }
}
