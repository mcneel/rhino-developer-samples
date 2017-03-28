using System.Diagnostics;
using Eto.Drawing;
using Eto.Forms;
using Rhino.UI;

namespace SampleCsEto.Views
{
  class SampleCsEtoOptionsPage : OptionsDialogPage
  {
    private SampleCsEtoOptionsPageControl m_page_control;

    public SampleCsEtoOptionsPage()
      : base("Sample")
    {
    }

    public override bool OnActivate(bool active)
    {
      return (m_page_control == null || m_page_control.OnActivate(active));
    }

    public override bool OnApply()
    {
      return (m_page_control == null || m_page_control.OnApply());
    }

    public override void OnCancel()
    {
      if (m_page_control != null)
        m_page_control.OnCancel();
    }

    public override object PageControl
    {
      get
      {
        return (m_page_control ?? (m_page_control = new SampleCsEtoOptionsPageControl()));
      }
    }

    public override System.Drawing.Image PageImage => Properties.Resources.SampleCsEtoPanelPng;
  }

  class SampleCsEtoOptionsPageControl : Panel
  {
    public SampleCsEtoOptionsPageControl()
    {
      var hello_button = new Button { Text = "Hello" };
      hello_button.Click += (sender, e) => OnHelloButton();

      var child_button = new Button { Text = "Child Dialog..." };
      child_button.Click += (sender, e) => OnChildButton();

      var layout = new DynamicLayout { DefaultSpacing = new Size(5, 5), Padding = new Padding(10) };
      layout.AddSeparateRow(hello_button, null);
      layout.AddSeparateRow(child_button, null);
      layout.Add(null);
      Content = layout;
    }

    public bool OnActivate(bool active)
    {
      Debug.WriteLine("SampleCsEtoOptionsDialogPage.OnActive(" + active + ")");
      return true;
    }

    public bool OnApply()
    {
      Debug.WriteLine("SampleCsEtoOptionsDialogPage.OnApply()");
      return true;
    }

    public void OnCancel()
    {
      Debug.WriteLine("SampleCsEtoOptionsDialogPage.OnCancel()");
    }

    /// <summary>
    /// Example of proper way to display a message box
    /// </summary>
    protected void OnHelloButton()
    {
      // Use the Rhino common message box and NOT the Eto MessageBox,
      // the Eto version expects a top level Eto Window as the owner for
      // the MessageBox and will cause problems when running on the Mac.
      // Since this panel is a child of some Rhino container it does not
      // have a top level Eto Window.
      Dialogs.ShowMessage("Hello Rhino!", "Sample");
    }

    /// <summary>
    /// Sample of how to display a child Eto dialog
    /// </summary>
    protected void OnChildButton()
    {
      var dialog = new SampleCsEtoHelloWorld();
      dialog.ShowModal(this);
    }
  }
}
