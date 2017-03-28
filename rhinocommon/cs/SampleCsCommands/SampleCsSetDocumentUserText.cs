using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  internal static class SampleCsDocStringData
  {
    public static string Key { get { return "SampleCsDocStringData"; } }
    public static string Value { get { return "Hello Rhino!"; } }
  }

  [System.Runtime.InteropServices.Guid("5C3C5D3F-123C-4A7A-A412-E240A8CB2D4C")]
  public class SampleCsSetDocumentUserText : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsSetDocumentUserText"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      doc.Strings.SetString(SampleCsDocStringData.Key, SampleCsDocStringData.Value);
      return Result.Success;
    }
  }

  [System.Runtime.InteropServices.Guid("E9F4A135-65B3-436D-A768-1AA7EE20DA0F")]
  public class SampleCsGetDocumentUserText : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsGetDocumentUserText"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var value = doc.Strings.GetValue(SampleCsDocStringData.Key);
      if (!string.IsNullOrEmpty(value))
        RhinoApp.WriteLine(string.Format("<{0}> {1}", SampleCsDocStringData.Key, value));
      return Result.Success;
    }
  }
}
