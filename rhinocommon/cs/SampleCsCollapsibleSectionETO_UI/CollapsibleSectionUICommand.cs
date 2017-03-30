using System;
using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace Project
{
  public class CollapsibleSectionUICommand : Command
  {
    public CollapsibleSectionUICommand()
    {
      // Rhino only creates one instance of each command class defined in a
      // plug-in, so it is safe to store a refence in a static property.
      Instance = this;
    }

    ///<summary>The only instance of this command.</summary>
    public static CollapsibleSectionUICommand Instance
    {
      get; private set;
    }

    ///<returns>The command name as it appears on the Rhino command line.</returns>
    public override string EnglishName
    {
      get { return "CollapsibleSectionUIExample"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // TODO: start here modifying the behaviour of your command.
      // ---
      System.Type panelType = typeof(CollapsibleSectionUIPanel);
      Rhino.UI.Panels.RegisterPanel(CollapsibleSectionUIPlugIn.Instance, panelType, "CollapsibleSectionUIExample", null);

      System.Guid panelId = CollapsibleSectionUIPanel.PanelId;
      bool bVisible = Rhino.UI.Panels.IsPanelVisible(panelId);

      if (!bVisible)
        Rhino.UI.Panels.OpenPanel(panelId);

      return Result.Success;
    }
  }
}
