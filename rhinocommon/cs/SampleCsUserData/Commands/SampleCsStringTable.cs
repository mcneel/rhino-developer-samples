using Rhino;
using Rhino.Commands;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsUserData.Commands
{
  public class SampleCsStringTable : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsStringTable"; }
    }

    private enum OptionIndex
    {
      AddOptionIndex = 1,
      DeleteOptionIndex = 2,
      ListOptionIndex = 3,
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetOption();
      go.SetCommandPrompt("Choose an option");
      go.AddOption("Add");
      go.AddOption("Delete");
      go.AddOption("List");
      go.AcceptNothing(true);

      var res = go.Get();

      Result rc;
      switch (res)
      {
      case GetResult.Option:
        rc = CommandOption(go.Option());
        break;
      case GetResult.Nothing:
        rc = Result.Success;
        break;
      default:
        rc = Result.Cancel;
        break;
      }

      return rc;
    }

    protected Result CommandOption(CommandLineOption option)
    {
      if (null == option)
        return Result.Failure;

      switch ((OptionIndex)option.Index)
      {
        case OptionIndex.AddOptionIndex:
          return AddOption();
        case OptionIndex.DeleteOptionIndex:
          return DeleteOption();
        case OptionIndex.ListOptionIndex:
          return ListOption();
      }

      return Result.Cancel;
    }

    protected Result AddOption()
    {
      var gs = new GetString();
      gs.SetCommandPrompt("String to add");
      gs.AcceptNothing(true);
      switch (gs.Get())
      {
        case GetResult.String:
          break;
        case GetResult.Nothing:
          return Result.Nothing;
        default:
          return Result.Cancel;
      }

      var str = gs.StringResult().Trim();
      if (string.IsNullOrEmpty(str))
        return Result.Nothing;

      var plugin = SampleCsUserDataPlugIn.Instance;
      if (plugin.StringDocumentDataTable.Add(str) < 0)
        RhinoApp.WriteLine("Unable to add string.");

      return Result.Success;
    }

    protected Result DeleteOption()
    {
      var gs = new GetString();
      gs.SetCommandPrompt("String to delete");
      switch (gs.Get())
      {
        case GetResult.String:
          break;
        case GetResult.Nothing:
          return Result.Nothing;
        default:
          return Result.Cancel;
      }

      var str = gs.StringResult().Trim();
      if (string.IsNullOrEmpty(str))
        return Result.Nothing;

      var plugin = SampleCsUserDataPlugIn.Instance;
      if (!plugin.StringDocumentDataTable.Delete(str))
        RhinoApp.WriteLine("Unable to delete string.");

      return Result.Success;
    }

    protected Result ListOption()
    {
      var plugin = SampleCsUserDataPlugIn.Instance;
      for (var i = 0; i < plugin.StringDocumentDataTable.Count; i++)
        RhinoApp.WriteLine(plugin.StringDocumentDataTable.Item(i));
      return Result.Success;
    }
  }
}
