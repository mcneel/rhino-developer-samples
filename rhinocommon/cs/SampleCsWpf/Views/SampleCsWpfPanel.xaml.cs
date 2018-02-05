using System.Windows.Controls;
using SampleCsWpf.ViewModels;

namespace SampleCsWpf.Views
{
  /// <summary>
  /// Interaction logic for SampleCsWpfPanel.xaml
  /// </summary>
  public partial class SampleCsWpfPanel
  {
    public SampleCsWpfPanel(uint documentSerialNumber)
    {
      DataContext = new SampleCsWpfPaneViewModel(documentSerialNumber);
      InitializeComponent();
    }

    private SampleCsWpfPaneViewModel ViewModel => DataContext as SampleCsWpfPaneViewModel;

    private void Button_Click(object sender, System.Windows.RoutedEventArgs e)
    {
      var vm = ViewModel;
      if (vm == null)
        return;

      for (var i = 0; i < 20; i++)
      {
        // Keep changing the same setting to make sure it only writes the file
        // one time when this loop is done
        vm.IncrementCounter();
        
        // Change a different settings each time which should reset the save
        // settings timer
        if (vm.UseMultipleCounters == true)
          vm.IncrementCounter(i);
      
        // The save timer fires every 500ms, this is here to make sure we go past
        // that a couple of times.  This is useful when testing the settings auto
        // writing function, it should only get called once when this loop is 
        // finished.
        System.Threading.Thread.Sleep(100);
      }

      vm.Message = $"Counter set to {vm.Counter}";
    }
  }
}
