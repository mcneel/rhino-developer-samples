using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace SampleCustomRenderSettingsSections
{
  public class DataSource
  {
    public event PropertyChangedEventHandler DataChanged;
    private bool m_check_box_value;

    public bool CheckBoxValue
    {
      get
      {
        return m_check_box_value;
      }

      set
      {
        if (value != m_check_box_value)
        {
          m_check_box_value = value;
          OnPropertyChanged();
        }
      }
    }

    public DataSource()
    {
      m_check_box_value = false;
    }

    void OnPropertyChanged([CallerMemberName] string memberName = null)
    {
      if (DataChanged != null)
      {
        DataChanged(this, new PropertyChangedEventArgs(memberName));
      }
    }
  }
}
