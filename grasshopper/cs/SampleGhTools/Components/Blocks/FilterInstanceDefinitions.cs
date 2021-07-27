using Grasshopper.Kernel;
using System;
using System.Collections.Generic;

namespace SampleGhTools.Components.Blocks
{
  /// <summary>
  /// Filter instance definitions
  /// </summary>
  public class FilterInstanceDefinitions : InstanceComponentBase
  {
    public FilterInstanceDefinitions()
      : base("Filter Instance Definitions", "FltIDefs", "Filter instance definitions.", "Sample", "Block")
    {
    }

    protected override void RegisterInputParams(GH_Component.GH_InputParamManager pManager)
    {
      pManager.AddTextParameter("IDefs", "ID", "Ids of instance definitions.", GH_ParamAccess.list);
      pManager.AddBooleanParameter("Hidden", "H", "Get hidden instance definitions.", GH_ParamAccess.item, true);
      pManager.AddBooleanParameter("Reference", "R", "Get instance definitions from reference files.", GH_ParamAccess.item, true);
      pManager.AddBooleanParameter("Used", "U", "Get used instance definitions.", GH_ParamAccess.item, true);
      pManager.AddBooleanParameter("Unused", "UN", "Get unused instance definitions.", GH_ParamAccess.item, true);
    }

    protected override void RegisterOutputParams(GH_Component.GH_OutputParamManager pManager)
    {
      pManager.AddTextParameter("IDefs", "ID", "Ids of instance definitions.", GH_ParamAccess.list);
    }

    protected override void SolveInstance(IGH_DataAccess DA)
    {
      if (null == DA) throw new ArgumentNullException(nameof(DA));

      var strings = new List<string>();
      var hidden = false;
      var reference = true;
      var used = true;
      var unused = true;

      if (!DA.GetDataList(0, strings)) return;
      if (!DA.GetData(1, ref hidden)) return;
      if (!DA.GetData(2, ref reference)) return;
      if (!DA.GetData(3, ref used)) return;
      if (!DA.GetData(4, ref unused)) return;

      var out_list = new List<string>(strings.Count);

      foreach (var str in strings)
      {
        var guid_str = str.Trim();

        var idef = InstanceDefinitionFromIdString(guid_str, true);
        if (null != idef)
        {
          var add = true;

          if (add)
          {
            if (!hidden && (guid_str[0] == '*'))
              add = false;
          }

          if (add)
          {
            if (!reference && idef.IsReference)
              add = false;
          }

          if (add)
          {
            if (!used && !unused)
              add = false;
            else if (used && !unused)
            {
              if (!idef.InUse(0))
                add = false;
            }
            else if (!used && unused)
            {
              if (idef.InUse(0))
                add = false;
            }
          }

          if (add)
            out_list.Add(guid_str);
        }
      }

      DA.SetDataList(0, out_list);
    }

    protected override System.Drawing.Bitmap Icon => Properties.Resources.FilterInstanceDefinitions_24x24;
    public override Guid ComponentGuid => new Guid("61acc590-4e1f-443f-9575-56056961dd3e");
  }
}