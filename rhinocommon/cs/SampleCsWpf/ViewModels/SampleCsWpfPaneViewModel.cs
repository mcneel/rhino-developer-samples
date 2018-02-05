namespace SampleCsWpf.ViewModels
{
  internal class SampleCsWpfPaneViewModel : Rhino.UI.ViewModel
  {
    public SampleCsWpfPaneViewModel(uint documentSerialNumber)
    {
      DocumentRuntimeSerialNumber = documentSerialNumber;
      SampleCsWpfPlugIn.Instance.SettingsSaved +=
        (sender, args) => SettingsChangedMessage = $"SettingsSaved {++m_saved_count}";
      m_use_multiple_counters = UseMultipleCounters == true;
    }

    private int m_saved_count;

    private uint DocumentRuntimeSerialNumber { get; }

    public string Message
    {
      get => m_message ?? string.Empty;
      set => SetProperty(value, ref m_message);
    }
    private string m_message;

    public string SettingsChangedMessage
    {
      get => m_settings_changed_message ?? string.Empty;
      set => SetProperty(value, ref m_settings_changed_message);
    }
    private string m_settings_changed_message;

    public bool? UseMultipleCounters
    {
      get => Settings.GetBool(nameof(UseMultipleCounters), false);
      set
      {
        m_use_multiple_counters = UseMultipleCounters == true;
        SetProperty(value == true, ref m_use_multiple_counters);
      }
    }
    private bool m_use_multiple_counters;

    public void IncrementCounter() => Counter = Counter + 1;

    public Rhino.PersistentSettings Settings => SampleCsWpfPlugIn.Instance.Settings;

    public int Counter
    {
      get => Settings.GetInteger(nameof(Counter), 0);
      set => Settings.SetInteger(nameof(Counter), value);
    }

    public void IncrementCounter(int index) => SetCounter(index, GetCounter(index) + 1);

    public string GetKey(int index) => $"Counter{index}";

    public int GetCounter(int index) => Settings.GetInteger(GetKey(index), 0);

    public void SetCounter(int index, int value) => Settings.SetInteger(GetKey(index), value);
  }
}