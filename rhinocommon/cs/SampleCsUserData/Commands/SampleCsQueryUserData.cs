using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input;

namespace SampleCsUserData.Commands
{
  [System.Runtime.InteropServices.Guid("171d4a92-600b-485c-9629-d5648a878600")]
  public class SampleCsQueryUserData : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsQueryUserData"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      const ObjectType filter = ObjectType.AnyObject;
      ObjRef objref;
      var rc = RhinoGet.GetOneObject("Select object", false, filter, out objref);
      if (rc != Result.Success || null == objref)
        return rc;

      RhinoObject obj = objref.Object();
      if (null == obj)
        return Result.Failure;

      var ud = obj.Attributes.UserData.Find(typeof(SampleCsUserDataObject)) as SampleCsUserDataObject;
      if (null != ud)
        RhinoApp.WriteLine("{0} = {1}", ud.Description, ud.Notes);
      else
        RhinoApp.WriteLine("User data not found.");

      return Result.Success;
    }
  }
}
