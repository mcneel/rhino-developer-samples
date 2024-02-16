using System;
using System.Security.Cryptography;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsPerFaceMaterial : Command
  {
    public override string EnglishName => "SampleCsPerFaceMaterial";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var render_plugin_id = Rhino.Render.Utilities.DefaultRenderPlugInId;
      var idi = new GuidIndex();

      var go = new GetObject();
      go.SetCommandPrompt("Select object with the rendering material you want to assign");
      go.EnablePreSelect(false, true);
      go.EnablePostSelect(true);
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var obj = go.Object(0).Object();
      if (null == obj)
        return Result.Failure;

      var att = obj.Attributes;
      if ((idi.Index = att.MaterialIndex) >= 0)
      {
        idi.Id = doc.Materials[idi.Index].Id;
      }
      else
      {
        MaterialRef mat_ref = null;
        if (att.MaterialRefs.ContainsKey(idi.Id))
          mat_ref = att.MaterialRefs[render_plugin_id];
        if (null == mat_ref)
            mat_ref = att.MaterialRefs[Guid.Empty];
        if (null != mat_ref)
        {
          idi.Id = mat_ref.FrontFaceMaterialId;
          idi.Index = doc.Materials.Find(idi.Id, true);
        }
      }

      if (!idi.IsValid)
      {
        RhinoApp.WriteLine("This object does not have a rendering material.");
        return Result.Nothing;
      }

      return Result.Success;
    }
  }

  internal class GuidIndex
  {
    public Guid Id { get; set; } = Guid.Empty;
    public int Index { get; set; } = RhinoMath.UnsetIntIndex;

    public bool IsValid => Id != Guid.Empty && Index >= 0;
  }
}