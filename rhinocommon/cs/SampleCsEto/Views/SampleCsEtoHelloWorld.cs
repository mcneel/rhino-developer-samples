using Eto.Drawing;
using Eto.Forms;
using Rhino.UI.Forms;

namespace SampleCsEto.Views
{
  class SampleCsEtoHelloWorld : CommandDialog
  {
    public SampleCsEtoHelloWorld()
    {
      Padding = new Padding(10);
      Title = "Hello World";
      Resizable = false;
      Content = new StackLayout()
      {
        Padding = new Padding(0),
        Spacing = 6,
        Items =
        {
          new Label { Text="This is a child dialog..." }
        }
      };
    }
  }
}
