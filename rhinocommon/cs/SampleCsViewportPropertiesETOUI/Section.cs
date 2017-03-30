using Rhino;
using Rhino.UI.Controls;

namespace SampleViewportPropertiesETOUI
{
  ///<summary>
  /// Base class for all the sections
  ///</summary>
  public abstract class Section : EtoCollapsibleSection
  {
    protected int m_table_padding = 10;

    public virtual void DisplayData(RhinoDoc doc)
    {
    }

    public virtual void EnableDisableControls()
    {
    }
  };
}
