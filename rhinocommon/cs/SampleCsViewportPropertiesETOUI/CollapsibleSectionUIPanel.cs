using System;
using Rhino;
using Rhino.Commands;
using Rhino.UI.Controls;
using Eto.Forms;

namespace SampleViewportPropertiesETOUI
{
  public class CollapsibleSectionUIPanel : Panel
  {

    /// <summary>
    /// Returns the ID of this panel.
    /// </summary>
    public static System.Guid PanelId
    {
      get
      {
        return typeof(CollapsibleSectionUIPanel).GUID;
      }
    }

    /// <summary>
    /// Public constructor
    /// </summary>
    public CollapsibleSectionUIPanel()
    {
      InitializeComponents();
      InitializeLayout();
    }

    private void InitializeComponents()
    {

    }


    private void InitializeLayout()
    {
      // Create holder for sections. The holder can expand/collaps sections and
      // displays a title for each section
      EtoCollapsibleSectionHolder holder = new EtoCollapsibleSectionHolder();

      // Create two sections
      SectionOne section1 = new SectionOne();
      SectionTwo section2 = new SectionTwo();

      // Populate the holder with sections
      holder.Add(section1);
      holder.Add(section2);

      // Create a tablelayout that contains the holder and add it to the UI
      // Content
      TableLayout tableLayout = new TableLayout()
      {
        Rows =
                {
                    holder
                }
      };

      Content = tableLayout;
    }
  }
}
