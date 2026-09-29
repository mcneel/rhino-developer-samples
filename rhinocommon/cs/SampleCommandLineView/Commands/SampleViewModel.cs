using Eto.Forms;
using Rhino;
using Rhino.Commands;
using Rhino.Input;
using Rhino.UI;
using Rhino.DocObjects;
using System;
using System.Collections.Generic;

namespace SampleCommandLineView
{
  // This is the ViewModel part of the MVVM (Model View ViewModel) architecture.
  // the view model provides business logic and validation and serves as the 
  // glue between the model (where the data lives) and the view (what the user sees)

  class SampleViewModel : ViewModel
  {
    internal RhinoDoc Document { get; }
    internal SampleModel Model { get; }
    public SampleViewModel(RhinoDoc doc, SampleModel model)
    {
      Document = doc;
      Model = model;
    }

    // the following properties are decorated with [Rhino* ...] attributes
    // that allow these properties to be edited from the commandline

    [RhinoInteger(1, Name = "Identification", Prompt = "Give a new Id", MinValue = 1)]
    public int Id
    {
      get => Model.Id;
      set
      {
        Model.Id = value;
        RaisePropertyChanged(nameof(Id));
      }
    }

    [RhinoNumber(2, MaxValue = 20.0)]
    public double Value
    {
      get => Model.Value;
      set
      {
        Model.Value = value;
        RaisePropertyChanged(nameof(Value));
      }
    }

    [RhinoToggle(3)]
    public bool Switch
    {
      get => Model.Switch;
      set
      {
        Model.Switch = value;
        RaisePropertyChanged(nameof(Switch));
      }
    }

    [RhinoEnum(4, typeof(Coloring), ChoicesProperty = nameof(AvailableColorings))]
    public Coloring Coloring
    {
      get => Model.Coloring;
      set
      {
        Model.Coloring = value;
        RaisePropertyChanged(nameof(Coloring));
      }
    }

    public IEnumerable<object> AvailableColorings
    {
      get
      {
        // this returns all available colorings but
        // could also return a sub-set of available colorings
        // to choose from.
        Array a = Enum.GetValues(typeof(Coloring));
        for (int i = 0; i < a.Length; i++)
        {
          yield return a.GetValue(i);
        }
      }
    }

    bool _advanced = false;

    [RhinoToggle(5)]
    public bool Advanced
    {
      get => _advanced;
      set
      {
        _advanced = value;
        _maxIterationsHasValue = _advanced;
        RaisePropertyChanged(nameof(Advanced));
        RaisePropertyChanged(nameof(MaxIterations));
        SelectPoints?.UpdateCanExecute();
      }
    }

    // the following property is only available
    // if its _maxIterationsHasValue is true (set when Advanced is changed)
    // all integer, number and toggle properties can be enabled/disabled in this way

    bool _maxIterationsHasValue = false;
    [RhinoInteger(6, Prompt = "Give maximum number of iterations")]
    public int? MaxIterations
    {
      get
      {
        if (_maxIterationsHasValue) return Model.MaxIterations;
        return default;
      }
      set
      {
        Model.MaxIterations = value.Value;
        _maxIterationsHasValue = true;
        RaisePropertyChanged(nameof(MaxIterations));
      }
    }

    // the next property is a command that is only executable when Advanced is true
    private RelayCommand<object> _selectPointsCommand;

    // the select points command can only be executed if Advanced is true
    [RhinoAction(7)]
    public RelayCommand<object> SelectPoints => _selectPointsCommand ??= new(DoSelectPoints, _ => Advanced);

    private void DoSelectPoints(object o)
    {
      string commandPrompt = RhinoApp.CommandPrompt;

      if (Result.Success == RhinoGet.GetMultipleObjects("Select points", false, ObjectType.Point, out var points))
      {
        foreach (var point in points)
          Document.Objects.Select(point, false, true);

        Document.Views.Redraw();
        RhinoApp.WriteLine($"{points.Length} points selected.");
      }

      // need to reset the command prompt
      RhinoApp.CommandPrompt = commandPrompt;
    }

    // the last property is a sub-viewmodel. this allows nesting of options
    // as this will fire up a GetOption to run the sub-viewmodel

    SampleSubViewModel _subVM;
    [RhinoChild(8, Prompt = "Sub menu options")]
    public SampleSubViewModel SubMenu => _subVM ??= new(Model);


    private RelayCommand _resetCommand;
    [RhinoAction(100)]
    public RelayCommand<object> Reset => _resetCommand ??= new(DoReset);

    private void DoReset()
    {
      Model.ResetToDefaults();
      foreach (var pi in GetType().GetProperties())
      {
        RaisePropertyChanged(pi.Name);
      }
    }

    public void Report()
    {
      RhinoApp.WriteLine(Model.Report());
    }
  }

  class SampleSubViewModel : ViewModel
  {
    public SampleModel Model { get; }

    public SampleSubViewModel(SampleModel model)
    {
      Model = model;
    }

    [RhinoInteger(0, MinValue = 0, Prompt = "Set the number of items")]
    public int NumberOfItems
    {
      get => Model.NumberOfItems;
      set
      {
        Model.NumberOfItems = value;
        RaisePropertyChanged(nameof(NumberOfItems));
      }
    }

    [RhinoToggle(1)]
    public bool Highlight
    {
      get => Model.Highlight;
      set
      {
        Model.Highlight = value;
        RaisePropertyChanged(nameof(Highlight));
      }
    }

  }
}
