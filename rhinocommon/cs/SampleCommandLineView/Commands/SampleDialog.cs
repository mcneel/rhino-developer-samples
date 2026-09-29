using Eto.Forms;
using Rhino.UI;
using Rhino.UI.Controls;
using Rhino.UI.Forms;

namespace SampleCommandLineView
{
  class SampleDialog : CommandDialog
  {
    public SampleDialog(SampleViewModel vm)
    {
      DataContext = vm;
      TableLayout layout = new RhinoTableLayout(RhinoLayout.PaddingType.RhinoPanel, RhinoLayout.SpacingType.Dialog);

      Label idLabel = new()
      {
        Text = nameof(vm.Id)
      };

      NumericStepper id = new NumericStepper
      {
        MinValue = 1,
        DataContext = vm
      };

      id.BindDataContext(v => v.Value, (SampleViewModel m) => m.Id);
      layout.Rows.Add(new TableRow(idLabel, id));

      Label valueLabel = new()
      {
        Text = nameof(vm.Value)
      };
      NumericStepper value = new()
      {
        MinValue = 0.0,
        MaxValue = 20.0,
        DecimalPlaces = 1
      };
      value.BindDataContext(v => v.Value, (SampleViewModel m) => m.Value);
      layout.Rows.Add(new TableRow(valueLabel, value));

      Label coloringLabel = new()
      {
        Text = nameof(vm.Coloring)
      };

      DropDown coloring = new();
      coloring.BindDataContext(c => c.DataStore, (SampleViewModel m) => m.AvailableColorings);
      coloring.SelectedValueBinding.BindDataContext<SampleViewModel>(m => m.Coloring);
      layout.Rows.Add(new TableRow(coloringLabel, coloring));

      Label switchLabel = new()
      {
        Text = nameof(vm.Switch)
      };

      CheckBox @switch = new();
      @switch.BindDataContext(c => c.Checked, (SampleViewModel m) => m.Switch);
      layout.Rows.Add(new TableRow(switchLabel, @switch));

      Label advancedLabel = new()
      {
        Text = nameof(vm.Advanced)
      };

      CheckBox advanced = new();
      advanced.BindDataContext(c => c.Checked, (SampleViewModel m) => m.Advanced);
      layout.Rows.Add(new TableRow(advancedLabel, advanced));

      NumericStepper iter = new()
      {
        MinValue = 0
      };

      // bind enable to the inverse of advanced
      iter.BindDataContext(c => c.Enabled, (SampleViewModel m) => m.Advanced).Source.Convert(v => !v);

      // bind the value to the converted nullable integer
      iter.BindDataContext<int?>(nameof(iter.Value), nameof(vm.MaxIterations)).Source.Convert(v => v.HasValue ? v.Value : 0);

      Label iterLabel = new()
      {
        Text = "Max. iterations"
      };

      layout.Rows.Add(new TableRow(iterLabel, iter));

      Button selectButton = new()
      {
        Text = "Select points",
        Command = vm.SelectPoints
      };

      Button resetButton = new()
      {
        Text = "Reset",
        Command = vm.Reset
      };


      layout.Rows.Add(new TableRow(selectButton));
      layout.Rows.Add(new TableRow(resetButton));

      // set the Rhino style so it follows e.g. Dark Mode
      layout.UseRhinoStyle();

      Content = layout;
    }
  }
}
