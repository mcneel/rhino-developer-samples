using System.Collections.Generic;
using Rhino;
using Rhino.UI;

namespace SampleCsEto
{
  public class SampleCsEtoPlugIn : Rhino.PlugIns.PlugIn
  {
    public SampleCsEtoPlugIn()
    {
      Instance = this;
    }

    public static SampleCsEtoPlugIn Instance
    {
      get;
      private set;
    }

    protected override void DocumentPropertiesDialogPages(RhinoDoc doc, List<OptionsDialogPage> pages)
    {
      var page = new Views.SampleCsEtoOptionsPage();
      pages.Add(page);
    }

    protected override void OptionsDialogPages(List<OptionsDialogPage> pages)
    {
      var page = new Views.SampleCsEtoOptionsPage();
      pages.Add(page);
    }

    protected override void ObjectPropertiesPages(List<ObjectPropertiesPage> pages)
    {
      var page = new Views.SampleCsEtoPropertiesPage();
      pages.Add(page);
    }
  }
}