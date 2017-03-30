using Rhino.UI.Controls;
using System;

namespace CustomRenderSections
{
  ///<summary>
  /// Base class for all the custom sections
  ///</summary>
  public abstract class CustomSection : EtoCollapsibleSection
  {
    private Guid m_uuidPlugIn = new Guid("54cc4233-7407-4c76-9422-0b6f01ca802a");

    public override Guid PlugInId
    {
      get { return m_uuidPlugIn; }
    }
    public virtual void OnViewModelActivated(object sender, EventArgs e)
    {
      DataContext = ViewModel;
    }

    public CustomSection()
    {
      ViewModelActivated += OnViewModelActivated;
    }
  };
}
