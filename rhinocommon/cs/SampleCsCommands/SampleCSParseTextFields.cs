using System;
using Rhino;
using Rhino.Commands;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsParseTextFields : Command
  {
    static SampleCsParseTextFields _instance;
    public SampleCsParseTextFields()
    {
      _instance = this;
    }

    ///<summary>The only instance of the SampleCSParseTextFields command.</summary>
    public static SampleCsParseTextFields Instance
    {
      get { return _instance; }
    }

    public override string EnglishName
    {
      get { return "SampleCSParseTextFields"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
     
      //Select some objects to add attribute user text to
      var go = new GetObject();
      go.SetCommandPrompt("Select objects");
      go.GroupSelect = true;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();


      //Apply the ObjectName textfield as the user text value to the selected objects
      for (var i = 0; i < go.ObjectCount; i++)
      {
        var obj_ref = go.Object(i);
        var obj = obj_ref.Object();
        if (null != obj)
        {
          //Create an ObjectName TextField and apply it as the value of the user text
          var fx = $"%<ObjectName(\"{obj_ref.ObjectId}\")>%";
          obj.Attributes.SetUserString("ObjectName", fx);
        }
      }



      //Now retrieve the values we just set and parse them
      for (var i = 0; i < go.ObjectCount; i++)
      {
        var obj_ref = go.Object(i);
        var obj = obj_ref.Object();
        if (null == obj)
          continue;

        //Read user text value and parse it as a textfield
        var user_string_value = obj.Attributes.GetUserString("ObjectName");
        if (!string.IsNullOrEmpty(user_string_value))
        {
          if (user_string_value.StartsWith("%<") && user_string_value.EndsWith(">%"))
          {
            //RhinoApp.ParseTextField will try to automatically parse any string that looks like a valid textfield containing
            //a formula. %<ObjectName("E3D64B7C-3AE7-4390-B983-82730091B8CD")>% for example and return the results.
            //The parent object parameter is required for block instances. 
            var parsed_string = RhinoApp.ParseTextField(user_string_value, obj_ref.Object(), null);
            RhinoApp.WriteLine($"Parsed TextField: {parsed_string}");
          }
        }

        //Direct method call to TextField ObjectName Example
        //You can also call the ObjectName and other TextField functions directly instead of using ParseTextField should you wish to do so.
        var direct_string = Rhino.Runtime.TextFields.ObjectName(obj_ref.ObjectId.ToString());
        var direct_area = Rhino.Runtime.TextFields.Area(obj_ref.ObjectId.ToString(), UnitSystem.Millimeters.ToString());
        RhinoApp.WriteLine($"Direct ObjectName call: {direct_string}");
        RhinoApp.WriteLine($"Direct Area call: {direct_area}");
      }






      return Result.Success;
    }
  }
}
