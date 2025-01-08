using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;

namespace SampleCsCommands
{
  public class SampleCsDumpBlockTree : Command
  {
    public override string EnglishName => "SampleCsDumpBlockTree";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Rhino.DocObjects.Tables.InstanceDefinitionTable idef_table = doc.InstanceDefinitions;
      int idef_count = idef_table.ActiveCount;
      if (0 == idef_count)
      {
        RhinoApp.WriteLine("SampleDumpBlockTree");
        return Result.Success;
      }

      int indent = 0;
      for (int i = 0; i < idef_count; i++)
        DumpInstanceDefinition(idef_table[i], ref indent);

      return Result.Success;
    }

    /// <summary>
    /// Recursive function to print the contents of an instance definition
    /// </summary>
    protected void DumpInstanceDefinition(InstanceDefinition idef, ref int indent)
    {
      if (null != idef && !idef.IsDeleted)
      {
        const string line = "\u2500";
        const string corner = "\u2514";

        string node = (0 == indent) ? line : corner;
        string str = new string(' ', indent * 2);
        RhinoApp.WriteLine($"{str}{node} Instance definition {idef.Index} = {idef.Name}");

        int idef_object_count = idef.ObjectCount;
        if (idef_object_count > 0)
        {
          indent++;
          str = new string(' ', indent * 2);
          for (int i = 0; i < idef_object_count; i++)
          {
            RhinoObject obj = idef.Object(i);
            if (null != obj)
            {
              if (obj is InstanceObject iref)
                DumpInstanceDefinition(iref.InstanceDefinition, ref indent);
              else
                RhinoApp.WriteLine($"{str}{corner} Object {i} = {obj.ShortDescription(false)}\n");
            }
          }
          indent--;
        }
      }
    }
  }
}
