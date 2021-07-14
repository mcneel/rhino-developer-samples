using System;
using Eto.Drawing;
using Eto.Forms;
using Rhino.UI;

namespace SampleCsEto.Views
{
  /// <summary>
  /// Required class GUID, used as the panel Id
  /// </summary>
  [System.Runtime.InteropServices.Guid("0E7780CA-F004-4AE7-B918-19E68BF7C7C9")]
  public class SampleCsEtoPanel : Panel, IPanel
  {
    readonly uint m_document_sn = 0;

    /// <summary>
    /// Provide easy access to the SampleCsEtoPanel.GUID
    /// </summary>
    public static System.Guid PanelId => typeof(SampleCsEtoPanel).GUID;

    /// <summary>
    /// Required public constructor with NO parameters
    /// </summary>
    public SampleCsEtoPanel(uint documentSerialNumber)
    {
      m_document_sn = documentSerialNumber;

      Title = GetType().Name;

      var hello_button = new Button { Text = "Hello..." };
      hello_button.Click += (sender, e) => OnHelloButton();

      var child_button = new Button { Text = "Child Dialog..." };
      child_button.Click += (sender, e) => OnChildButton();

      var document_sn_label = new Label() { Text = $"Document serial number: {documentSerialNumber}" };

      var layout = new DynamicLayout { DefaultSpacing = new Size(5, 5), Padding = new Padding(10) };
      layout.AddSeparateRow(hello_button, null);
      layout.AddSeparateRow(child_button, null);
      layout.AddSeparateRow(document_sn_label, null);
      layout.Add(null);
      Content = layout;
    }


    public string Title { get; }

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
      Dialogs.ShowMessage("Hello Rhino!", Title);
    }

    /// <summary>
    /// Sample of how to display a child Eto dialog
    /// </summary>
    protected void OnChildButton()
    {
      var dialog = new SampleCsEtoHelloWorld();
      dialog.ShowModal(this);
    }

    #region IPanel methods
    public void PanelShown(uint documentSerialNumber, ShowPanelReason reason)
    {
      // Called when the panel tab is made visible, in Mac Rhino this will happen
      // for a document panel when a new document becomes active, the previous
      // documents panel will get hidden and the new current panel will get shown.
      Rhino.RhinoApp.WriteLine($"Panel shown for document {documentSerialNumber}, this serial number {m_document_sn} should be the same");
    }

    public void PanelHidden(uint documentSerialNumber, ShowPanelReason reason)
    {
      // Called when the panel tab is hidden, in Mac Rhino this will happen
      // for a document panel when a new document becomes active, the previous
      // documents panel will get hidden and the new current panel will get shown.
      Rhino.RhinoApp.WriteLine($"Panel hidden for document {documentSerialNumber}, this serial number {m_document_sn} should be the same");
    }

    public void PanelClosing(uint documentSerialNumber, bool onCloseDocument)
    {
      // Called when the document or panel container is closed/destroyed
      Rhino.RhinoApp.WriteLine($"Panel closing for document {documentSerialNumber}, this serial number {m_document_sn} should be the same");
    }
    #endregion IPanel methods
  }
}
