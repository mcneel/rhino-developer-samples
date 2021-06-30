using Grasshopper.Kernel;
using System;
using System.Collections.Generic;

namespace SampleGhTools.Components.Blocks
{
  /// <summary>
  /// Gets the properties of an instance definition
  /// </summary>
  public class InstanceDefinitionProperties : InstanceComponentBase
  {
    public InstanceDefinitionProperties()
      : base("Instance Definition Properties", "IDefProps", "Gets the properties of an instance definition.", "Sample", "Block")
    {
    }

    protected override void RegisterInputParams(GH_Component.GH_InputParamManager pManager)
    {
      pManager.AddTextParameter("IDef", "ID", "Id of the instance definition.", GH_ParamAccess.item);
    }

    protected override void RegisterOutputParams(GH_Component.GH_OutputParamManager pManager)
    {
      pManager.AddTextParameter("Name", "Name", "Name of the instance definition", GH_ParamAccess.item);
      pManager.AddTextParameter("Objects", "O", "Ids of instance definition objects.", GH_ParamAccess.list);
      pManager.AddTextParameter("References", "R", "Ids of instance definition references.", GH_ParamAccess.list);
      pManager.AddTextParameter("Containers", "C", "Ids of instance definitions that reference to this definition.", GH_ParamAccess.list);
    }

    protected override void SolveInstance(IGH_DataAccess DA)
    {
      if (null == DA) throw new ArgumentNullException(nameof(DA));

      var str = "";
      if (!DA.GetData(0, ref str)) return;

      var guid_str = str.Trim();
      var idef = InstanceDefinitionFromIdString(guid_str, true);
      if (null != idef)
      {
        DA.SetData(0, idef.Name);

        var rhino_objects = idef.GetObjects();
        if (null != rhino_objects)
        {
          var out_list = new List<string>(rhino_objects.Length);
          foreach (var rh_obj in rhino_objects)
          {
            if (null != rh_obj)
              out_list.Add(rh_obj.Id.ToString());
          }
          DA.SetDataList(1, out_list);
        }

        var instance_references = idef.GetReferences(0);
        if (null != instance_references)
        {
          var out_list = new List<string>(instance_references.Length);
          foreach (var iref in instance_references)
          {
            if (null != iref)
              out_list.Add(iref.Id.ToString());
          }
          DA.SetDataList(2, out_list);
        }

        var containing_definitions = idef.GetContainers();
        if (null != containing_definitions)
        {
          var out_list = new List<string>(containing_definitions.Length);
          foreach (var cdef in containing_definitions)
          {
            if (null != cdef)
              out_list.Add(cdef.Id.ToString());
          }
          DA.SetDataList(3, out_list);
        }
      }
    }

    protected override System.Drawing.Bitmap Icon => Properties.Resources.InstanceDefinitionProperties_24x24;
    public override Guid ComponentGuid => new Guid("e02cac2a-6120-45c2-bd8e-81844d7f8c63");
  }
}