using Rhino;
using Rhino.PlugIns;
using Rhino.Runtime;
using Rhino.UI;
using System.Collections.Generic;

namespace SampleCsEto
{
  public class SampleCsEtoPlugIn : Rhino.PlugIns.PlugIn
  {
    // Name of the callback that this plug-in registers with Rhino so that
    // native (C++) plug-ins can show this Eto dialog. The companion C++ sample
    // (SampleCppEtoInterop, command SampleCppEtoRebuildCurve) calls
    // RhinoExecuteNamedCallback using this same string. See RH-92307.
    private const string RebuildCurveCallbackName = "SampleCsEtoRebuildCurveDialog";

    public SampleCsEtoPlugIn()
    {
      Instance = this;
    }

    public static SampleCsEtoPlugIn Instance
    {
      get;
      private set;
    }

    /// <summary>
    /// Called when the plug-in is loaded. Registers a named callback that a C++
    /// plug-in can invoke - by way of the public CRhinoParameterDictionary and
    /// RhinoExecuteNamedCallback SDK (rhinoSdkInterop.h) - to display the Eto
    /// "Rebuild" dialog from native code and get the user's choices back.
    /// </summary>
    protected override LoadReturnCode OnLoad(ref string errorMessage)
    {
      HostUtils.RegisterNamedCallback(RebuildCurveCallbackName, OnRebuildCurveDialogCallback);
      return base.OnLoad(ref errorMessage);
    }

    /// <summary>
    /// Called when the plug-in is unloaded. Removes the named callback.
    /// </summary>
    protected override void OnShutdown()
    {
      HostUtils.RemoveNamedCallback(RebuildCurveCallbackName);
      base.OnShutdown();
    }

    /// <summary>
    /// Named callback that bridges a native (C++) caller to the managed Eto
    /// "Rebuild" dialog. The C++ caller fills the dictionary with the inputs
    /// listed below, this handler shows the dialog, and the user's choices are
    /// written back into the same dictionary for the C++ caller to read.
    ///
    /// Inputs  (C++ -> dialog): PointCount (int), Degree (int),
    ///                          DeleteInput (bool), PreserveTangents (bool)
    /// Outputs (dialog -> C++): Result (bool, true when OK was pressed), plus
    ///                          the edited PointCount, Degree, DeleteInput and
    ///                          PreserveTangents values.
    /// </summary>
    private void OnRebuildCurveDialogCallback(object sender, NamedParametersEventArgs e)
    {
      Commands.RebuildCurveArgs args = new Commands.RebuildCurveArgs();
      if (e.TryGetInt("PointCount", out int pointCount))
        args.PointCount = pointCount;
      if (e.TryGetInt("Degree", out int degree))
        args.Degree = degree;
      if (e.TryGetBool("DeleteInput", out bool deleteInput))
        args.DeleteInput = deleteInput;
      if (e.TryGetBool("PreserveTangents", out bool preserveTangents))
        args.PreserveTangents = preserveTangents;

      Commands.RebuildCurveDialog dialog = new Commands.RebuildCurveDialog(args);
      bool ok = dialog.ShowModal(RhinoEtoApp.MainWindow);

      e.Set("Result", ok);
      if (ok)
      {
        args = dialog.Results;
        e.Set("PointCount", args.PointCount);
        e.Set("Degree", args.Degree);
        e.Set("DeleteInput", args.DeleteInput);
        e.Set("PreserveTangents", args.PreserveTangents);
      }
    }

    protected override void DocumentPropertiesDialogPages(RhinoDoc doc, List<OptionsDialogPage> pages)
    {
      Views.SampleCsEtoOptionsPage page = new Views.SampleCsEtoOptionsPage();
      pages.Add(page);
    }

    protected override void OptionsDialogPages(List<OptionsDialogPage> pages)
    {
      Views.SampleCsEtoOptionsPage page = new Views.SampleCsEtoOptionsPage();
      pages.Add(page);
    }

    protected override void ObjectPropertiesPages(ObjectPropertiesPageCollection collection)
    {
      Views.SampleCsEtoPropertiesPage page = new Views.SampleCsEtoPropertiesPage();
      collection.Add(page);
    }
  }
}