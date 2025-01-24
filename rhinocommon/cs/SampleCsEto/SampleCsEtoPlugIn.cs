using Rhino;
using Rhino.UI;
using System.Collections.Generic;

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
      Views.SampleCsEtoOptionsPage page = new Views.SampleCsEtoOptionsPage();
      pages.Add(page);
    }

    protected override void OptionsDialogPages(List<OptionsDialogPage> pages)
    {
      Views.SampleCsEtoOptionsPage page = new Views.SampleCsEtoOptionsPage();
      pages.Add(page);
    }

    protected override void ObjectPropertiesPages(ObjectPropertiesPageCollection collection)
    {
      Views.SampleCsEtoPropertiesPage page = new Views.SampleCsEtoPropertiesPage();
      collection.Add(page);
    }
  }
}