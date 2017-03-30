using Rhino.UI.Controls;
using System;

namespace CustomSunSections
{
  ///<summary>
  /// Base class for all the custom sections
  ///</summary>
  public abstract class CustomSection : EtoCollapsibleSection
  {
    private Guid m_uuidPlugIn = new Guid("1a44e9a2-9c77-4eb5-8c15-ad1a6d4bea9b");

    public override Guid PlugInId
    {
      get { return m_uuidPlugIn; }
    }

  };
}
