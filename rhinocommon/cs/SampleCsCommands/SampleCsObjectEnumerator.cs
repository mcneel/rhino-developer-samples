using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsObjectEnumerator : Command
  {
    private const string Normal = "Normal";
    private const string Locked = "Locked";
    private const string Hidden = "Hidden";
    private const string Active = "Active";
    private const string Reference = "Reference";
    private const string OffValue = "No";
    private const string OnValue = "Yes";

    public override string EnglishName => "SampleCsObjectEnumerator";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Get persistent settings
      var bNormal = Settings.GetBool(Normal, true);
      var bLocked = Settings.GetBool(Locked, true);
      var bHidden = Settings.GetBool(Hidden, true);
      var bActive = Settings.GetBool(Active, true);
      var bReference = Settings.GetBool(Reference, true);

      // Create command line options
      var optNormal = new OptionToggle(bNormal, OffValue, OnValue);
      var optLocked = new OptionToggle(bLocked, OffValue, OnValue);
      var optHidden = new OptionToggle(bHidden, OffValue, OnValue);
      var optActive = new OptionToggle(bActive, OffValue, OnValue);
      var optReference = new OptionToggle(bReference, OffValue, OnValue);

      var go = new GetOption();
      go.SetCommandPrompt("Object enumerator options");
      go.AcceptNothing(true);
      while (true)
      {
        go.ClearCommandOptions();
        var idxNormal = go.AddOptionToggle(Normal, ref optNormal);
        var idxLocked = go.AddOptionToggle(Locked, ref optLocked);
        var idxHidden = go.AddOptionToggle(Hidden, ref optHidden);
        var idxActive = go.AddOptionToggle(Active, ref optActive);
        var idxReference = go.AddOptionToggle(Reference, ref optReference);

        // Get the options
        var res = go.Get();

        if (res == GetResult.Option)
        {
          var index = go.Option().Index;
          if (index == idxNormal)
            bNormal = optNormal.CurrentValue;
          else if (index == idxLocked)
            bLocked = optLocked.CurrentValue;
          else if (index == idxHidden)
            bHidden = optHidden.CurrentValue;
          else if (index == idxActive)
            bActive = optActive.CurrentValue;
          else if (index == idxReference)
            bReference = optReference.CurrentValue;
          continue;
        }

        if (res != GetResult.Nothing)
          return Result.Cancel;

        if (!bNormal && !bLocked && !bHidden)
        {
          var msg = string.Format("Either \"{0}\" or \"{1}\" or \"{2}\" must be \"{3}\"", 
            Normal, 
            Locked, 
            Hidden,
            OnValue
            );
          RhinoApp.WriteLine(msg);
          continue;
        }

        if (!bActive && !bReference)
        {
          var msg = string.Format("Either \"{0}\" or \"{1}\" must be \"{2}\"", 
            Active, 
            Reference, 
            OnValue
            );
          RhinoApp.WriteLine(msg);
          continue;
        }

        break;
      }

      var settings = new ObjectEnumeratorSettings
      {
        NormalObjects = bNormal,
        LockedObjects = bLocked,
        HiddenObjects = bHidden,
        ActiveObjects = bActive,
        ReferenceObjects = bReference,
        IncludeLights = false,
        IncludeGrips = false
      };

      var objects = doc.Objects.GetObjectList(settings);
      foreach (var obj in objects)
      {
        var msg = string.Format("{0}, {1}, {2}, {3}",
          obj.Id,
          obj.ShortDescription(false), 
          obj.IsNormal ? Normal : obj.IsLocked ? Locked : Hidden,
          obj.IsReference ? Reference : Active
          );
        RhinoApp.WriteLine(msg);
      }

      // Set persistent setings
      Settings.SetBool(Normal, bNormal);
      Settings.SetBool(Locked, bLocked);
      Settings.SetBool(Hidden, bHidden);
      Settings.SetBool(Active, bActive);
      Settings.SetBool(Reference, bReference);

      return Result.Success;
    }
  }
}