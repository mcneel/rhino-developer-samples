using System;
using System.Windows.Forms;
using System.Drawing;
using Rhino.Display;
using Rhino.Render;
using Rhino.Render.UI;
using System.Collections.Generic;
using Rhino.UI;
using Rhino.UI.Controls;
using Rhino.UI.Controls.DataSource;
using System.Diagnostics;

namespace rdktest_cs
{
  [System.Runtime.InteropServices.Guid("49864F18-905C-4160-8C74-2AB2CCAD6649")]
  public partial class CustomMaterialUserInterfaceSection : RhinoWindows.Controls.RenderContentUserControlCollapsibleSection
  {
    public CustomMaterialUserInterfaceSection()
    {
      InitializeComponent();

      m_view_model = new CustomMaterialViewModel(this);

      subNodeControl.Section = this;

      //Hook up the data display to the changed event
      DataChanged += UserInterfaceDisplayData;
      ControlAdded += CustomMaterialUserInterfaceSection_ControlAdded;
    }

    private void CustomMaterialUserInterfaceSection_ControlAdded(object sender, ControlEventArgs e)
    {
      height = Size.Height;
    }

    public override LocalizeStringPair Caption
    {
      get
      {
        return new LocalizeStringPair("Custom Section: Parameters", "Custom Section: Parameters");
      }
    }
    private int height = 300;
    public override int SectionHeight
    {
      get
      {
        return height;
      }
    }


    #region private members
    // This is the same type of view model used by the WPF version of the user control
    private readonly CustomMaterialViewModel m_view_model;
    #endregion private members

    public void UserInterfaceDisplayData(object sender, Rhino.UI.Controls.DataSource.EventArgs e)
    {
      // Update the "Show section 2" check box state
      bool? bShowSection2 = m_view_model.VariesShowSection2;
      if (bShowSection2 != null)
      {
        checkBox1.ThreeState = false;
        checkBox1.CheckState = bShowSection2.Value ? CheckState.Checked : CheckState.Unchecked;
      }
      else
      {
        checkBox1.ThreeState = true;
        checkBox1.CheckState = CheckState.Indeterminate;
      }

      // Set the color button's background color
      Color4f? bc = m_view_model.VariesColor;
      if (bc != null)
      {
        buttonColor.BackColor = bc.Value.AsSystemColor();
      }
      else
      {
        buttonColor.BackColor = Color.White;
      }

      buttonColor.Text = m_view_model.ColorText;
      buttonColor.Refresh();

      subNodeControl.ParentInstanceList = m_view_model.SelectedMaterialIdList;
    }

    #region Control events
    private void ButtonColorClick(object sender, System.EventArgs e)
    {
      Color4f color = m_view_model.VariesColor.Value;

      if (!Rhino.UI.Dialogs.ShowColorDialog((object)this, ref color, false))
        return;

      m_view_model.VariesColor = color;
    }

    private void CheckBox1CheckedChanged(object sender, System.EventArgs e)
    {
      switch (checkBox1.CheckState)
      {
        case CheckState.Checked:
          m_view_model.ShowSection2 = true;
          break;
        case CheckState.Unchecked:
          m_view_model.ShowSection2 = false;
          break;
        case CheckState.Indeterminate:
          Debug.Assert(false);
          break;
      }
    }

    #endregion Control events
  }
}
