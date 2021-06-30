using Grasshopper.Kernel;
using System;

namespace SampleGhTools.Components.Blocks
{
  /// <summary>
  /// Gets the properties of an instance reference object
  /// </summary>
  public class InstanceObjectProperties : InstanceComponentBase
  {
    public InstanceObjectProperties()
      : base("Instance Object Properties", "IRefProps", "Gets the properties of an instance reference object.", "Sample", "Block")
    {
    }

    protected override void RegisterInputParams(GH_Component.GH_InputParamManager pManager)
    {
      pManager.AddTextParameter("IDef", "ID", "Id of the instance definition.", GH_ParamAccess.item);
    }

    protected override void RegisterOutputParams(GH_Component.GH_OutputParamManager pManager)
    {
      pManager.AddPointParameter("Point", "P", "Insertion point", GH_ParamAccess.item);
      pManager.AddTransformParameter("Transform", "T", "Transformation", GH_ParamAccess.item);
    }

    protected override void SolveInstance(IGH_DataAccess DA)
    {
      if (null == DA) throw new ArgumentNullException(nameof(DA));

      var str = "";
      if (!DA.GetData(0, ref str)) return;

      var guid_str = str.Trim();
      var iref = InstanceObjectFromIdString(guid_str, true);
      if (null != iref)
      {
        DA.SetData(0, iref.InsertionPoint);
        DA.SetData(1, iref.InstanceXform);
      }
    }

    protected override System.Drawing.Bitmap Icon => Properties.Resources.InstanceObjectProperties_24x24;
    public override Guid ComponentGuid => new Guid("43b05ed7-247b-4c9c-9843-b2934de539e9");
  }
}