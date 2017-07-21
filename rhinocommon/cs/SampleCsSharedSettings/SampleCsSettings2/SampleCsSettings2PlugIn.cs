using System;
using Rhino.PlugIns;

namespace SampleCsSettings2
{
  public class SampleCsSettings2PlugIn : PlugIn
  {
    private bool m_rhino_done;

    private Guid SampleCsSettings1PlugInId => new System.Guid("7f2d0955-4e2d-4854-b128-cee498cfdd5c");

    public SampleCsSettings2PlugIn()
    {
      Instance = this;
    }

    public static SampleCsSettings2PlugIn Instance
    {
      get; private set;
    }

    public override PlugInLoadTime LoadTime => PlugInLoadTime.AtStartup;

    protected override LoadReturnCode OnLoad(ref string errorMessage)
    {
      var thread = new System.Threading.Thread(LoadAndRegister) { Name = "Registering thread" };
      Rhino.RhinoApp.Initialized += OnRhinoAppInitialized;
      thread.Start();
      return LoadReturnCode.Success;
    }

    private void OnRhinoAppInitialized(object sender, System.EventArgs e)
    {
      m_rhino_done = true;
    }

    private void LoadAndRegister()
    {
      while (!m_rhino_done)
        System.Threading.Thread.Sleep(10);

      // Find SampleCsSettings1 plug-in
      var plugin_id = SampleCsSettings1PlugInId;
      var plugin = Find(SampleCsSettings1PlugInId);
      if (plugin == null)
      {
        if (LoadPlugIn(plugin_id))
          plugin = Find(plugin_id);
      }

      if (plugin != null)
        plugin.SettingsSaved += OnPlugInSettingsSaved;
    }

    private void OnPlugInSettingsSaved(object sender, Rhino.PersistentSettingsSavedEventArgs e)
    {
      Rhino.RhinoApp.WriteLine("Settings in SampleCsSettings1 saved");
      ReadSetting();
    }

    public void ReadSetting()
    {
      // Find SampleCsSettings1 plug-in
      var plugin = Find(SampleCsSettings1PlugInId);
      if (plugin != null)
      {
        var value = plugin.Settings.GetInteger("SomeSetting", -1);
        Rhino.RhinoApp.WriteLine($"Setting from SampleCsSettings1 > {value}");
      }
    }
  }
}