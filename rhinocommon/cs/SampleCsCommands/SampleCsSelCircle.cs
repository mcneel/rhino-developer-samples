using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;

namespace SampleCsCommands
{
  /// <summary>
  /// Demonstrates how to create selection, or "Sel", commands
  /// </summary>
  public class SampleCsSelCircle : SelCommand
  {
    public override string EnglishName
    {
      get { return "SampleCsSelCircle"; }
    }

    protected override bool SelFilter(RhinoObject rhObj)
    {
      if (null != rhObj)
      { 
        var curve_obj = rhObj as CurveObject;
        if (null != curve_obj)
        {
          var doc = rhObj.Document;
          var curve = curve_obj.CurveGeometry;
          if (null != doc && null != curve)
            return curve.TryGetCircle(out Circle circle, doc.ModelAbsoluteTolerance);
        }
      }
      return false;
    }
  }
}