using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace SampleCsDragDrop
{
  [Guid("5CB2DA85-3BA6-4180-A04F-280EF8326B53")]
  public partial class SampleCsUserControl : UserControl
  {
    private bool m_dragging;
    private int m_drag_item_index = -1;

    public static Guid PanelId => typeof(SampleCsUserControl).GUID;

    public SampleCsUserControl()
    {
      InitializeComponent();
      m_listbox.Items.Add("One");
      m_listbox.Items.Add("Two");
      m_listbox.Items.Add("Three");
      m_listbox.Items.Add("Four");
      m_listbox.Items.Add("Five");
    }

    private void OnListBoxMouseDown(object sender, MouseEventArgs e)
    {
      m_drag_item_index = m_listbox.IndexFromPoint(e.Location);
    }

    private void OnListBoxMouseMove(object sender, MouseEventArgs e)
    {
      var item_index = m_listbox.IndexFromPoint(e.Location);
      if (m_drag_item_index < 0 || m_dragging || item_index == m_drag_item_index)
        return;

      m_dragging = true;

      Rhino.RhinoApp.WriteLine("DoDragDrop...");
      var result = m_listbox.DoDragDrop(this, DragDropEffects.All);
      Rhino.RhinoApp.WriteLine($"...DoneDragDrop({result})");

      m_dragging = false;
      m_drag_item_index = -1;
    }

    public string DropString => m_drag_item_index < 0 ? "<empty>" : m_listbox.Items[m_drag_item_index] as string;
  }
}
