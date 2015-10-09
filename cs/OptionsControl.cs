using System;
using System.Windows.Forms;

namespace examples_cs
{
  partial class OptionsControl : UserControl
  {
    public OptionsControl()
    {
      InitializeComponent();
    }

    private void OptionsControl_Load(object sender, EventArgs e)
    {
      //throw new Exception("My exception");
    }
  }

  class CustomObjPropPage : Rhino.UI.ObjectPropertiesPage
  {
    OptionsControl m_control = new OptionsControl();


    public override System.Drawing.Icon Icon
    {
      get { return Properties.Resources.mapping_match; }
    }

    public override Control PageControl
    {
      get { return m_control; }
    }

    public override string EnglishPageTitle
    {
      get { return "Awesome props"; }
    }
  }

  class CustomOptionPage : Rhino.UI.OptionsDialogPage
  {
    OptionsControl m_control = new OptionsControl();
    public CustomOptionPage()
      : base("CS_OptionPage")
    { }

    public CustomOptionPage(string name) : base(name) { }

    public override Control PageControl
    {
      get { return null; }
    }

    public override bool OnActivate(bool active)
    {
      return base.OnActivate(active);
    }

    IntPtr ChildHwndSourceHook(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
    {
      const int WM_GETDLGCODE = 0x0087;
      const int DLGC_WANTCHARS = 0x0080;
      const int DLGC_WANTARROWS = 0x0001;
      const int DLGC_HASSETSEL = 0x0008;

      if (msg == WM_GETDLGCODE)
      {
        handled = true;
        return new IntPtr(DLGC_WANTCHARS | DLGC_WANTARROWS | DLGC_HASSETSEL);
      }
      return IntPtr.Zero;
    }


    System.Windows.Interop.HwndSource m_source;
    readonly WpfOptionsPage m_page = new WpfOptionsPage();
    public override void OnCreateParent(IntPtr hwndParent)
    {
      var parameters = new System.Windows.Interop.HwndSourceParameters("CS_Options");
      parameters.PositionX = 0;
      parameters.PositionY = 0;
      parameters.Height = (int)m_page.Height;
      parameters.Width = (int)m_page.Width;
      parameters.ParentWindow = hwndParent;
      const int WS_CHILD = 0x40000000;
      const int WS_VISIBLE = 0x10000000;

      parameters.WindowStyle =  WS_VISIBLE | WS_CHILD;

      m_source = new System.Windows.Interop.HwndSource(parameters);
      m_source.SizeToContent = System.Windows.SizeToContent.WidthAndHeight;
      m_source.AddHook(ChildHwndSourceHook);
      if( m_source.CompositionTarget!=null )
        m_source.CompositionTarget.BackgroundColor = System.Windows.SystemColors.WindowBrush.Color;
      m_source.RootVisual = m_page;
    }
    private static readonly Action EmptyDelegate = delegate { };

    public override void OnSizeParent(int cx, int cy)
    {
      m_page.Width = cx;
      m_page.Height = cy;
      if (m_source != null)
      {
        m_page.Dispatcher.Invoke(System.Windows.Threading.DispatcherPriority.Render, EmptyDelegate);
      }
    }

    public override bool OnApply()
    {
      return base.OnApply();
    }

    public override void OnCancel()
    {
      base.OnCancel();
    }

    public override void OnDefaults()
    {
      base.OnDefaults();
    }

    public override void OnHelp()
    {
      base.OnHelp();
    }
  }

}
