using System.Runtime.InteropServices;
using Eto.Forms;

namespace SampleCsWizardPanel
{
  /// <summary>
  /// A MainPanel will be created for each new Rhino document and will bet 
  /// disposed of when the document closes
  /// </summary>
  [Guid("5b2184b1-7dca-43bf-8c4b-6a5ce2c82649")]
  public class MainPanel : Panel
  {
    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="documentRuntimeSerialNumber">
    /// RhinoDoc.RuntimeSerialNumber associated with this MainPanel instance.
    /// </param>
    public MainPanel(uint documentRuntimeSerialNumber)
    {
      // Padding around the main container, child panels should use a padding 
      // of 0
      Padding = 6;
      // ViewModel associated with a specific RhinoDoc.RuntimeSerialNumber
      var view = new SampleCsWizardPanelViewModel(documentRuntimeSerialNumber);
      // Set this panels DataContext, Page... panels will inherit this
      // DeviceContext
      DataContext = view;
      // Bind this panel's content to the view model, the Next and Back
      // buttons will set this property.
      this.Bind(c => c.Content, view, m => m.Content);
    }
  }
}
