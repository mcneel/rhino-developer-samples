using System;
using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsSettings2
{
  public class SampleCsSettings2Command : Command
  {
    public override string EnglishName => "SampleCsSettings2";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      SampleCsSettings2PlugIn.Instance.ReadSetting();
      return Result.Success;
    }
  }
}
