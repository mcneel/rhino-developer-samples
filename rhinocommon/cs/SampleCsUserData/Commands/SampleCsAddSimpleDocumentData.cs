using Rhino;
using Rhino.Commands;
using Rhino.Input;

namespace SampleCsUserData.Commands
{
  [System.Runtime.InteropServices.Guid("dd97883a-077d-4ea3-9848-8752ac40339b")]
  public class SampleCsAddSimpleDocumentData : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsAddSimpleDocumentData"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var data_table = SampleCsUserDataPlugIn.Instance.SimpleDocumentDataTable;
      if (0 == data_table.Count)
      {
        var number = 6;
        var rc = RhinoGet.GetInteger("Number of data objects to create", true, ref number, 1, 100);
        if (rc != Result.Success)
          return rc;

        for (var i = 0; i < number; i++)
          data_table.Add(new SampleCsSimpleDocumentData());
      }

      for (var i = 0; i < data_table.Count; i++)
        RhinoApp.WriteLine("Data[{0}] = {1}", i, data_table[i].Value);

      return Result.Success;
    }
  }
}
