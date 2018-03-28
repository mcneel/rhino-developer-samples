using System;
using System.Collections.Generic;
using System.Linq;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input;

namespace SampleCsCommands
{
  public class SampleCsGroup : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsGroup"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      ObjRef[] objrefs;
      var rc = RhinoGet.GetMultipleObjects("Select objects to group", false, ObjectType.AnyObject, out objrefs);
      if (rc != Rhino.Commands.Result.Success)
        return rc;
      if (objrefs == null || objrefs.Length < 1)
        return Result.Failure;

      var guids = new List<Guid>(objrefs.Length);
      guids.AddRange(objrefs.Select(objref => objref.ObjectId));

      doc.Groups.Add(guids);
      doc.Views.Redraw();
      
      return Result.Success;
    }
  }
}
