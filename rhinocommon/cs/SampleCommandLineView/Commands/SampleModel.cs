using Rhino;
using System.Text;

namespace SampleCommandLineView
{
  enum Coloring
  {
    GreyWhite,
    Blueish,
    PurpleDots,
    Rainbow
  }

  // This is the Model of the MVVM (Model View ViewModel) architecture
  // the model is used to store data with simple get/set properties.
  // additionally the model can be used to (de)serialize the data. In this
  // case the (de)serialization is done by writing to PersistentSettings,
  // but any form of storage (XML, JSON, database) can be supported.

  class SampleModel
  {

    public int Id { get; set; } = 1;
    public double Value { get; set; } = 0;
    public bool Switch { get; set; } = true;
    public Coloring Coloring { get; set; } = Coloring.GreyWhite;
    public int MaxIterations { get; set; } = 10;
    public int NumberOfItems { get; set; } = 0;
    public bool Highlight { get; set; }

    #region I/O of the data

    // increase this version if the data model ever changes (see below)
    private static int SettingsVersion = 1;

    public void ResetToDefaults()
    {
      SampleModel @default = new();
      foreach (var pi in GetType().GetProperties())
      {
        if (pi.CanRead && pi.CanWrite)
        {
          pi.SetValue(this, pi.GetValue(@default));
        }
      }
    }

    public static SampleModel ReadFrom(PersistentSettings settings)
    {
      SampleModel model = new();

      // read the version of the data model. If the data model ever needs to change
      // you can add code here to support data saved in older versions.
      if (settings.TryGetInteger(nameof(SettingsVersion), out int version))
      {
        if (version != 1)
          return model;
      }

      // read the data by its name. This could also be done by JSON string representation
      // or any other means to deserialize the data. Here we use the existing command
      // Settings to read from.
      if (settings.TryGetInteger(nameof(Id), out int id))
        model.Id = id;
      if (settings.TryGetDouble(nameof(Value), out double value))
        model.Value = value;
      if (settings.TryGetBool(nameof(Switch), out bool @switch))
        model.Switch = @switch;
      if (settings.TryGetEnumValue<Coloring>(nameof(Coloring), out Coloring coloring))
        model.Coloring = coloring;
      if (settings.TryGetInteger(nameof(MaxIterations), out int maxIterations))
        model.MaxIterations = maxIterations;
      if (settings.TryGetInteger(nameof(NumberOfItems), out int numberOfItems))
        model.NumberOfItems = numberOfItems;
      if (settings.TryGetBool(nameof(Highlight), out bool highlight))
        model.Highlight = highlight;

      return model;
    }

    public void WriteTo(PersistentSettings settings)
    {
      settings.SetInteger(nameof(SettingsVersion), SettingsVersion);
      settings.SetInteger(nameof(Id), Id);
      settings.SetDouble(nameof(Value), Value);
      settings.SetBool(nameof(Switch), Switch);
      settings.SetEnumValue(nameof(Coloring), Coloring);
      settings.SetInteger(nameof(MaxIterations), MaxIterations);
      settings.SetInteger(nameof(NumberOfItems), NumberOfItems);
      settings.SetBool(nameof(Highlight), Highlight);
    }

    #endregion

    public string Report()
    {
      StringBuilder sb = new();
      foreach (var pi in GetType().GetProperties())
      {
        sb.AppendLine($"{pi.Name} = {pi.GetValue(this)}");
      }
      return sb.ToString();
    }
  }

}