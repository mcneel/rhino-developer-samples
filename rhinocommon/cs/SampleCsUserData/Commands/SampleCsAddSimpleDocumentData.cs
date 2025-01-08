using Rhino;
using Rhino.Commands;
using Rhino.Input;

namespace SampleCsUserData.Commands
{
  public class SampleCsAddSimpleDocumentData : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsAddSimpleDocumentData"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      SampleCsSimpleDocumentDataTable data_table = SampleCsUserDataPlugIn.Instance.SimpleDocumentDataTable;
      if (0 == data_table.Count)
      {
        int number = 6;
        Result rc = RhinoGet.GetInteger("Number of data objects to create", true, ref number, 1, 100);
        if (rc != Result.Success)
          return rc;

        for (int i = 0; i < number; i++)
          data_table.Add(new SampleCsSimpleDocumentData());
      }

      for (int i = 0; i < data_table.Count; i++)
        RhinoApp.WriteLine("Data[{0}] = {1}", i, data_table[i].Value);

      return Result.Success;
    }
  }
}
