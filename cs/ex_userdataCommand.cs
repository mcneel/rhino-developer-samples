using System;
using Rhino;
using System.Runtime.InteropServices;

namespace examples_cs
{
  // You must define a Guid attribute for your user data derived class
  // in order to support serialization. Every custom user data class
  // needs a custom Guid
  [Guid("7098A105-CD3C-4192-A9AC-0F21017098DC")]
  public class MyCustomData : Rhino.DocObjects.Custom.UserData
  {
    public int IntegerData{ get; set; }
    public string StringData {get; set;}

    // Your UserData class must have a public parameterless constructor
    public MyCustomData(){}

    public MyCustomData(int i, string s)
    {
      IntegerData = i;
      StringData = s;
    }

    public override string Description
    {
      get { return "Some Custom Properties"; }
    }

    public override string ToString()
    {
      return String.Format("integer={0}, string={1}", IntegerData, StringData);
    }

    protected override void OnDuplicate(Rhino.DocObjects.Custom.UserData source)
    {
      MyCustomData src = source as MyCustomData;
      if (src != null)
      {
        IntegerData = src.IntegerData;
        StringData = src.StringData;
      }
    }

    // return true if you have information to save
    public override bool ShouldWrite
    {
      get
      {
        // make up some rule as to if this should be saved in the 3dm file
        if (IntegerData > 0 && !string.IsNullOrEmpty(StringData))
          return true;
        return false;
      }
    }

    protected override bool Read(Rhino.FileIO.BinaryArchiveReader archive)
    {
      Rhino.Collections.ArchivableDictionary dict = archive.ReadDictionary();
      if (dict.ContainsKey("IntegerData") && dict.ContainsKey("StringData"))
      {
        IntegerData = (int)dict["IntegerData"];
        StringData = dict["StringData"] as String;
      }
      return true;
    }
    protected override bool Write(Rhino.FileIO.BinaryArchiveWriter archive)
    {
      // you can implement File IO however you want... but the dictionary class makes
      // issues like versioning in the 3dm file a bit easier.  If you didn't want to use
      // the dictionary for writing, your code would look something like.
      //
      //  archive.Write3dmChunkVersion(1, 0);
      //  archive.WriteInt(IntegerData);
      //  archive.WriteString(StringData);
      var dict = new Rhino.Collections.ArchivableDictionary(1, "MyCustomData");
      dict.Set("IntegerData", IntegerData);
      dict.Set("StringData", StringData);
      archive.WriteDictionary(dict);
      return true;
    }
  }


  [Guid("ca9a110e-3969-49ec-9d59-a7c2ee0b85bd")]
  public class ExUserdataCommand : Rhino.Commands.Command
  {
    public override string EnglishName { get { return "cs_userdataCommand"; } }

    protected override Rhino.Commands.Result RunCommand(RhinoDoc doc, Rhino.Commands.RunMode mode)
    {
      Rhino.DocObjects.ObjRef objref;
      var rc = Rhino.Input.RhinoGet.GetOneObject("Select Face", false, Rhino.DocObjects.ObjectType.Surface, out objref);
      if (rc != Rhino.Commands.Result.Success)
        return rc;

      var face = objref.Face();

      // See if user data of my custom type is attached to the geomtry
      // We need to use the underlying surface in order to get the user data
      // to serialize with the file.
      var ud = face.UnderlyingSurface().UserData.Find(typeof(MyCustomData)) as MyCustomData;
      if (ud == null)
      {
        // No user data found; create one and add it
        int i = 0;
        rc = Rhino.Input.RhinoGet.GetInteger("Integer Value", false, ref i);
        if (rc != Rhino.Commands.Result.Success)
          return rc;

        ud = new MyCustomData(i, "This is some text");
        face.UnderlyingSurface().UserData.Add(ud);
      }
      else
      {
        RhinoApp.WriteLine("{0} = {1}", ud.Description, ud);
      }
      return Rhino.Commands.Result.Success;
    }
  }
}

