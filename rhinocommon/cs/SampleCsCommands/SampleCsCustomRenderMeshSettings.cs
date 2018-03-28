using System;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  public class SampleCsCustomRenderMeshSettings : Command
  {
    public SampleCsCustomRenderMeshSettings()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsCustomRenderMeshSettings"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Rhino.Geometry.MeshingParameters mp = doc.GetMeshingParameters(Rhino.Geometry.MeshingParameterStyle.Custom);
      double relativeTolerance = mp.RelativeTolerance;

      Rhino.Input.Custom.GetNumber gn = new Rhino.Input.Custom.GetNumber();
      gn.SetCommandPrompt("Custom render mesh density");
      gn.SetDefaultNumber(relativeTolerance);
      gn.SetLowerLimit(0.0, false);
      gn.SetUpperLimit(1.0, false);
      gn.AcceptNothing(true);
      gn.Get();
      if (gn.CommandResult() != Result.Success)
        return gn.CommandResult();

      double newTolerance = gn.Number();
      if (newTolerance == relativeTolerance)
        return Result.Nothing;

      Rhino.Geometry.MeshingParameters new_mp = new Rhino.Geometry.MeshingParameters();
      new_mp = mp;
      new_mp.RelativeTolerance = newTolerance;

      doc.SetCustomMeshingParameters(new_mp);

      // Toggling the meshing parameter style will destroy existing render meshes,
      // which will be recreated when the scene is redrawn.
      if (doc.MeshingParameterStyle == Rhino.Geometry.MeshingParameterStyle.Custom)
        doc.MeshingParameterStyle = Rhino.Geometry.MeshingParameterStyle.Fast;
      doc.MeshingParameterStyle = Rhino.Geometry.MeshingParameterStyle.Custom;
      
      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
