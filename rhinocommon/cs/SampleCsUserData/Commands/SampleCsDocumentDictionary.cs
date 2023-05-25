using Rhino;
using Rhino.Collections;
using Rhino.Commands;

namespace SampleCsUserData.Commands
{
  public class SampleCsDocumentDictionary : Command
  {
    private int IntegerValue { get; set; } = 1;
    private double DoubleValue { get; set; } = 2.13;

    public override string EnglishName => "SampleCsDocumentDictionary";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetValues(SampleCsUserDataPlugIn.Instance.Dictionary);
      SetValues(SampleCsUserDataPlugIn.Instance.Dictionary);
      return Result.Success;
    }

    private void GetValues(ArchivableDictionary dictionary)
    {
      if (null == dictionary)
        return;

      RhinoApp.WriteLine(dictionary.Name);

      if (dictionary.TryGetInteger(nameof(IntegerValue), out var n))
      {
        IntegerValue = n;
        RhinoApp.WriteLine("  Integer = {0}", IntegerValue);
      }
      else
      {
        RhinoApp.WriteLine("  Integer = <none>");
      }

      if (dictionary.TryGetDouble(nameof(DoubleValue), out var d))
      {
        DoubleValue = d;
        RhinoApp.WriteLine("  Double = {0}", DoubleValue);
      }
      else
      {
        RhinoApp.WriteLine("  Double = <none>");
      }
    }

    private void SetValues(ArchivableDictionary dictionary)
    {
      if (null == dictionary)
        return;

      IntegerValue++;
      DoubleValue *= 1.57;

      dictionary.Set(nameof(IntegerValue), IntegerValue);
      dictionary.Set(nameof(DoubleValue), DoubleValue);
    }
  }
}