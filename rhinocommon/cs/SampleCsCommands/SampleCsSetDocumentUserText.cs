using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  internal static class SampleCsDocStringData
  {
    public static string Key => "SampleCsDocStringData";
    public static string Value => "Hello Rhino!";
  }

  public class SampleCsSetDocumentUserText : Command
  {
    public override string EnglishName => "SampleCsSetDocumentUserText";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      doc.Strings.SetString(SampleCsDocStringData.Key, SampleCsDocStringData.Value);
      return Result.Success;
    }
  }

  public class SampleCsGetDocumentUserText : Command
  {
    public override string EnglishName => "SampleCsGetDocumentUserText";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var value = doc.Strings.GetValue(SampleCsDocStringData.Key);
      if (!string.IsNullOrEmpty(value))
        RhinoApp.WriteLine($"<{SampleCsDocStringData.Key}> {value}");
      return Result.Success;
    }
  }
}
