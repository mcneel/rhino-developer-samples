using System.ComponentModel;
using System.Runtime.CompilerServices;
using Rhino.UI.Controls;
using Rhino.Render;
using Rhino.Collections;


namespace SampleCustomRenderSettingsSections
{
  public class CustomRenderSettingsViewModel : CollapsibleSectionViewModel, INotifyPropertyChanged
  {
    public event PropertyChangedEventHandler PropertyChanged;

    private Rhino.Render.DataSources.RhinoSettings RenderSettingsForRead()
    {
      return GetData(Rhino.UI.Controls.DataSource.ProviderIds.RhinoSettings, false, true) as Rhino.Render.DataSources.RhinoSettings;
    }

    private Rhino.Render.DataSources.RhinoSettings RenderSettingsForWrite()
    {
      return GetData(Rhino.UI.Controls.DataSource.ProviderIds.RhinoSettings, true, true) as Rhino.Render.DataSources.RhinoSettings;
    }

    private void CommitRenderSettings()
    {
      Commit(Rhino.UI.Controls.DataSource.ProviderIds.RhinoSettings);
    }

    // The CheckBoxValue is a custom boolean User Data
    // value for Render Settings
    public bool? CheckBoxValue
    {
      get
      {
        var rs = RenderSettingsForRead();

        bool value = false;

        Rhino.Render.RenderSettings render_settings = rs.GetRenderSettings();
        ArchivableDictionary userdata = render_settings.UserDictionary;
        if (!userdata.TryGetBool("BoolValue", out value))
          return false;

        return value;
      }

      set
      {
        if (value != CheckBoxValue)
        {
          if (value != null)
          {
            using (var u = UndoHelper("Custom Render Section 1 BoolValue changed"))
            {
              var rs = RenderSettingsForWrite();

              Rhino.Render.RenderSettings render_settings = rs.GetRenderSettings();
              ArchivableDictionary userdata = render_settings.UserDictionary;
              userdata.Set("BoolValue", (bool)value);
              rs.SetRenderSettings(render_settings);

              CommitRenderSettings();
              OnPropertyChanged();
            }
          }
        }
      }
    }

    public CustomRenderSettingsViewModel(ICollapsibleSection section)
      : base(section)
    {

    }

    void OnPropertyChanged([CallerMemberName] string memberName = null)
    {
      // ViewModel -> UI
      if (PropertyChanged != null)
      {
        PropertyChanged(this, new PropertyChangedEventArgs(memberName));
      }
    }
  }
}