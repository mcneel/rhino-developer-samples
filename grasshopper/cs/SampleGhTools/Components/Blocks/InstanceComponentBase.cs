using Grasshopper.Kernel;
using Rhino;
using Rhino.DocObjects;
using System;

namespace SampleGhTools.Components.Blocks
{
  /// <summary>
  /// Abstract base class for instance definition components
  /// </summary>
  public abstract class InstanceComponentBase : GH_Component
  {
    /// <summary>
    /// Constructor
    /// </summary>
    protected InstanceComponentBase(string name, string nickname, string description, string category, string subCategory)
      : base(name, nickname, description, category, subCategory)
    {
    }

    /// <summary>
    /// Get the active document
    /// </summary>
    public RhinoDoc Document => RhinoDoc.ActiveDoc;

    /// <summary>
    /// Gets an instance definition from the document
    /// </summary>
    public InstanceDefinition InstanceDefinitionFromId(Guid id, bool ignoreDeleted)
    {
      var idef = Document?.InstanceDefinitions.FindId(id);
      if (null != idef)
      {
        if (ignoreDeleted && idef.IsDeleted)
          idef = null;
      }
      return idef;
    }

    /// <summary>
    /// Gets an instance definition from the document
    /// </summary>
    public InstanceDefinition InstanceDefinitionFromIdString(string str, bool ignoreDeleted)
    {
      if (!string.IsNullOrWhiteSpace(str))
      {
        if (Guid.TryParse(str, out var id))
          return InstanceDefinitionFromId(id, ignoreDeleted);
      }
      return null;
    }

    /// <summary>
    /// Gets an instance object from the document
    /// </summary>
    public InstanceObject InstanceObjectFromId(Guid id, bool ignoreDeleted)
    {
      InstanceObject idef = null;
      var rh_obj = Document?.Objects.FindId(id);
      if (null != rh_obj)
      {
        idef = rh_obj as InstanceObject;
        if (null != idef)
        {
          if (ignoreDeleted && idef.IsDeleted)
            idef = null;
        }
      }
      return idef;
    }

    /// <summary>
    /// Gets an instance object from the document
    /// </summary>
    public InstanceObject InstanceObjectFromIdString(string str, bool ignoreDeleted)
    {
      if (!string.IsNullOrWhiteSpace(str))
      {
        if (Guid.TryParse(str, out var id))
          return InstanceObjectFromId(id, ignoreDeleted);
      }
      return null;
    }
  }
}
