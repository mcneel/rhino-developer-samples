using Grasshopper.Kernel;
using Rhino.DocObjects;
using System;
using System.Collections.Generic;

namespace SampleGhTools.Components.Blocks
{
  /// <summary>
  /// Gets instance definitions in the document
  /// </summary>
  public class InstanceDefinitions : InstanceComponentBase
  {
    public InstanceDefinitions()
      : base("Instance Definitions", "IDefs", "Gets instance definitions in the document.", "Sample", "Block")
    {
    }

    protected override void RegisterInputParams(GH_Component.GH_InputParamManager pManager)
    {
      pManager.AddBooleanParameter("Embedded", "E", "Get embedded instance definitions.", GH_ParamAccess.item, true);
      pManager.AddBooleanParameter("LinkedEmbedded", "LE", "Get linked and embedded instance definitions.", GH_ParamAccess.item, true);
      pManager.AddBooleanParameter("Linked", "L", "Get linked instance definitions.", GH_ParamAccess.item, true);
    }

    protected override void RegisterOutputParams(GH_Component.GH_OutputParamManager pManager)
    {
      pManager.AddTextParameter("IDefs", "ID", "Ids of instance definitions.", GH_ParamAccess.list);
    }

    protected override void SolveInstance(IGH_DataAccess DA)
    {
      if (null == DA) throw new ArgumentNullException(nameof(DA));

      var doc = Document;
      if (null == doc) return;

      var embedded = true;
      var linked_embedded = true;
      var linked = true;

      if (!DA.GetData(0, ref embedded)) return;
      if (!DA.GetData(1, ref linked_embedded)) return;
      if (!DA.GetData(2, ref linked)) return;

      var idefs = doc.InstanceDefinitions.GetList(true);
      Array.Sort(idefs, (idef0, idef1) => idef0.Id.CompareTo(idef1.Id));

      var out_list = new List<string>(idefs.Length);
      foreach (var idef in idefs)
      {
        if (embedded && idef.UpdateType == InstanceDefinitionUpdateType.Static
          || linked_embedded && idef.UpdateType == InstanceDefinitionUpdateType.LinkedAndEmbedded
          || linked && idef.UpdateType == InstanceDefinitionUpdateType.Linked
          )
          out_list.Add(idef.Id.ToString());
      }

      DA.SetDataList(0, out_list);
    }

    protected override System.Drawing.Bitmap Icon => Properties.Resources.InstanceDefinitions_24x24;
    public override Guid ComponentGuid => new Guid("d4338a16-e0ff-4947-9bda-4fb2ee9c833b");
  }
}