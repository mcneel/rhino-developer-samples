using System;
using System.Windows.Forms;
using Grasshopper.Kernel;

namespace SampleGhOnLoad
{
  public class SampleGhOnLoadAssemblyPriority : GH_AssemblyPriority
  {
    public override GH_LoadingInstruction PriorityLoad()
    {
      // show message box (could also be a full blown form) and ask the user a question
      if (MessageBox.Show("Do you want to load this sample?", "SampleGhOnLoad", MessageBoxButtons.YesNo) == DialogResult.Yes)
      {
        // success? carry on
        return GH_LoadingInstruction.Proceed;
      }

      // failure? skip loading this plug-in
      return GH_LoadingInstruction.Abort;
    }
  }
}
