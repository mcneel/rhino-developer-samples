using System;
using System.Drawing;
using System.Collections.Generic;
using System.Diagnostics;

using Rhino.Render.UI;
using Rhino.Render;
using RhinoWindows.ViewModels;

using Rhino.UI.Controls;

using Rhino.Display;
using Rhino.UI;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace SampleCsRdk
{
  class CustomMaterialViewModel : ViewModelBase
  {

    public CustomMaterialViewModel(Rhino.UI.Controls.ICollapsibleSection section)
    {
      m_user_interface_section = section;
    }

    public CustomMaterialViewModel()
    {
      m_user_interface_section = null;
    }

    public ICollapsibleSection Section
    {
      get
      {
        return m_user_interface_section;
      }
      set
      {
        m_user_interface_section = value;
      }
    }

    public RenderContentCollection Selection
    {
      get
      {
        if (m_user_interface_section == null || m_user_interface_section.ViewModel == null)
          return null;

        return m_user_interface_section.ViewModel.GetData(Rhino.UI.Controls.DataSource.ProviderIds.ContentSelection, false, false) as Rhino.Render.RenderContentCollection;
      }
    }

    public Color Color
    {
      get
      {
        var vc = VariesColor;
        if (null != vc)
        {
          return vc.Value.AsSystemColor();
        }
        return Color.White;
      }
      set
      {
        VariesColor = new Color4f(value);
      }
    }

    //Returns null if the color varies or there were no materials in the selection
    public Color4f? VariesColor
    {
      get
      {
        //Prepare the color values
        bool bColorVaries = false;
        Color4f? color = null;

        RenderContentCollection sel = Selection;
        if (sel == null)
          return null;

        foreach (var item in sel)
        {
          if (item is RenderMaterial material)
          {

            Color4f colorThis;
            if (!material.Fields.TryGetValue(COLOR_FIELD_NAME, out colorThis))
            {
              continue;
            }

            //First time through
            if (color == null)
            {
              color = colorThis;
            }
            else if (!bColorVaries && colorThis != color)
            {
              bColorVaries = true;
              break;
            }
          }
        }

        return bColorVaries ? null : color;
      }
      set
      {
        RenderContentCollection sel = Selection;
        if (sel == null)
          return;

        var undo = Rhino.RhinoDoc.ActiveDoc.BeginUndoRecord("Set material Color value");

        foreach (var item in sel)
        {
          if (item is RenderMaterial material)
          {

            Color4f colorThis;
            if (!material.Fields.TryGetValue(COLOR_FIELD_NAME, out colorThis))
            {
              Debug.Assert(false);
              continue;
            }

            if (value != colorThis)
            {
              material.BeginChange(RenderContent.ChangeContexts.UI);
              material.Fields.Set(COLOR_FIELD_NAME, value.Value);
              material.EndChange();
            }
          }
        }

        Rhino.RhinoDoc.ActiveDoc.EndUndoRecord(undo);
      }
    }

    public bool ShowSection2
    {
      get
      {
        var vc = VariesShowSection2;
        if (null != vc)
        {
          return vc.Value;
        }
        return false;
      }
      set
      {
        VariesShowSection2 = new bool?(value);
      }
    }

    //Returns null if the show flag varies or there were no materials in the selection
    public bool? VariesShowSection2
    {
      get
      {
        //Prepare the color values
        bool bVaries = false;
        bool? b = null;

        RenderContentCollection sel = Selection;
        if (sel == null)
          return null;

        foreach (var item in sel)
        {

          if (item is RenderMaterial material)
          {

            bool bThis;
            if (!material.Fields.TryGetValue(CustomMaterialWithUserControl.ShowSectionKey, out bThis))
            {
              continue;
            }

            //First time through
            if (b == null)
            {
              b = bThis;
            }
            else if (!bVaries && bThis != b)
            {
              bVaries = true;
              break;
            }
          }
        }

        return bVaries ? null : b;
      }
      set
      {
        RenderContentCollection sel = Selection;
        if (sel == null)
          return;

        var undo = Rhino.RhinoDoc.ActiveDoc.BeginUndoRecord("Set material ShowSectionKey value");

        foreach (var item in sel)
        {
          if (item is RenderMaterial material)
          {

            bool bThis;
            if (!material.Fields.TryGetValue(CustomMaterialWithUserControl.ShowSectionKey, out bThis))
            {
              Debug.Assert(false);
              continue;
            }

            if (value != bThis)
            {
              material.BeginChange(RenderContent.ChangeContexts.UI);
              material.Fields.Set(CustomMaterialWithUserControl.ShowSectionKey, value.Value);
              material.EndChange();
            }
          }
        }

        Rhino.RhinoDoc.ActiveDoc.EndUndoRecord(undo);
      }
    }

    public Guid[] SelectedMaterialIdList
    {
      get
      {
        var list = new List<Guid>();

        RenderContentCollection sel = Selection;
        if (sel != null)
        {
          foreach (var item in sel)
          {
            if (item is RenderMaterial material)
            {
              list.Add(material.Id);
            }
          }
        }

        return list.ToArray();
      }
      set
      {
        //Do nothing
      }
    }

    public string ColorText
    {
      get
      {
        return Color == null ? "varies" : "";
      }
      set
      {
        //Do nothing
      }
    }

    public string DiffuseChildSlotName
    {
      get { return m_diffuse_child_slot; }
      set { }
    }

    public void RaisePropertyChanged(Rhino.UI.Controls.DataSource.EventArgs e)
    {
      RaisePropertyChanged("Color");
      RaisePropertyChanged("ShowSection2");
      RaisePropertyChanged("Colortext");
      RaisePropertyChanged("SelectedMaterialIdList");
      RaisePropertyChanged("DiffuseChildSlotName");
    }

    #region Private memebers
    private const string COLOR_FIELD_NAME = "color";
    private ICollapsibleSection m_user_interface_section;
    private string m_diffuse_child_slot = "diffuse-child-slot";
    #endregion Private memebers
  }





  [System.Runtime.InteropServices.Guid("8CEADF9B-DB37-4CD4-A6C1-FA2283F0556B")]
  public class CustomMaterialWpfUserInterfaceSectionHost : RenderContentWpfCollapsibleSectionHost
  {
    public CustomMaterialWpfUserInterfaceSectionHost()
      : base(new CustomMaterialWpfUserInterfaceSection(), null)
    {
      var vm = WpfViewModel as CustomMaterialViewModel;
      vm.Section = this;

      DataChanged += UserInterfaceDisplayData;
    }

    public void UserInterfaceDisplayData(object sender, Rhino.UI.Controls.DataSource.EventArgs e)
    {
      var vm = WpfViewModel as CustomMaterialViewModel;

      vm.RaisePropertyChanged(e);
    }

    public override LocalizeStringPair Caption
    {
      get
      {
        return new LocalizeStringPair("WPF Section", "WPF section");
      }
    }

    public override int SectionHeight
    {
      get
      {
        return (int)FrameworkElement.ActualHeight;
      }
    }
  }
}

