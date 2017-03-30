using System;
using Rhino.UI.Controls;
using Eto.Forms;
using Rhino.UI;

namespace CustomSunSections
{
  ///<summary>
  /// The UI implementation of of Section one
  ///</summary>
  public class CustomSunSection2 : CustomSection
  {
    private Label m_sun_section2_label;
    private LocalizeStringPair m_caption;

    ///<summary>
    /// The Heigth of the section
    ///</summary>
    public override int SectionHeight
    {
      get
      {
        return 40;
      }
    }

    public override LocalizeStringPair Caption
    {
      get { return m_caption; }
    }

    ///<summary>
    /// Constructor for SectionOne
    ///</summary>
    public CustomSunSection2()
    {
      m_caption = new LocalizeStringPair("Custom Sun Section2", "Custom Sun Section2");
      InitializeComponents();
      InitializeLayout();
      RegisterControlEvents();
    }

    private void InitializeComponents()
    {

      m_sun_section2_label = new Label()
      {
        Text = "This is the sun section 2",
        VerticalAlignment = VerticalAlignment.Center,
      };
    }


    private void InitializeLayout()
    {
      TableLayout layout = new TableLayout()
      {
        // Padding around the table
        Padding = 10,
        // Spacing between table cells
        Spacing = new Eto.Drawing.Size(15, 15),
        Rows =
                {
                    new TableRow(m_sun_section2_label)
                }
      };
      Content = layout;
    }

    private void RegisterControlEvents()
    {

    }

    private void UnRegisterControlEvents()
    {

    }
  }
}
