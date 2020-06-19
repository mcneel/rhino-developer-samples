using System.Collections;
using System.Collections.Generic;
using System.Windows.Forms;

namespace SampleCsWinForms.Forms
{
  /// <summary>
  /// Helper class to handle WinForm form control focus when user presses 'Tab' key.
  /// </summary>
  /// <remarks>
  /// When displaying a custom WinForm form, you may have noticed than your form will
  /// not honor the tab indices you may have defined when designing the form in the 
  /// Visual Studio editor. Actually pressing the Tab key will have no effect.
  /// 
  /// The reason is that Rhino is setting some low level hook mechanism to intercept
  /// keypress events such as Tab or Enter, so you won’t be able to directly redirect
  /// those events to your controls unless you use some low level workarounds.
  /// 
  /// The code below illustrates a way to handle and respond to Tab keypress events
  /// with some custom logic. Just instantiate the  class and let it initialize an
  /// internal dictionary with the controls tab indices you defined at design time
  /// using Control.TabIndex property.
  /// </remarks>
  /// <example>
  /// public partial class MyForm : Form
  /// {
  ///   private SampleModelessTabFix m_tab_fix;
  ///
  ///   public MyForm()
  ///   {
  ///     InitializeComponent();
  ///     m_tab_fix = new SampleModelessTabFix(this);
  ///   }
  ///
  ///   // The rest of MyForm class implementation...
  /// }
  /// </example>
  public class SampleModelessTabFix
  {
    private readonly Form m_form;
    private readonly SortedDictionary<int, Control> m_controls;
    private SortedDictionary<int, Control>.Enumerator m_enumerator;

    /// <summary>
    /// Public constructor
    /// </summary>
    /// <param name="form">The form to manage.</param>
    public SampleModelessTabFix(Form form)
    {
      m_form = form;
      m_form.KeyPreview = true;
      m_controls = new SortedDictionary<int, Control>();
      if (LoadControlsTabIndices(form.Controls))
      {
        m_form.KeyPress += OnKeyPress;
        m_enumerator = m_controls.GetEnumerator();
      }
    }

    /// <summary>
    /// Loads tab indices recursively if form contains container controls.
    /// </summary>
    private bool LoadControlsTabIndices(IEnumerable controls)
    {
      try
      {
        foreach (Control ctrl in controls)
        {
          if (!(ctrl is ContainerControl) && !(ctrl is GroupBox) && !(ctrl is Panel))
          {
            var index = ctrl.TabIndex;
            while (m_controls.ContainsKey(index))
              ++index;
            m_controls.Add(index, ctrl);
          }
          LoadControlsTabIndices(ctrl.Controls);
        }
        return true;
      }
      catch
      {
        return false;
      }
    }

    /// <summary>
    /// Finds the nested control with focus. In case of container controls,
    /// iterate to find the control that really has focus,such as a TextBox
    /// inside a GroupBox.
    /// </summary>
    private Control GetActiveControl()
    {
      if (null != m_form.ActiveControl)
      {
        var ctrl = m_form.ActiveControl;
        while (ctrl is ContainerControl container)
          ctrl = container.ActiveControl;
        return ctrl;
      }
      return null;
    }

    /// <summary>
    /// Initializes the control enumerator to the active control.
    /// </summary>
    private void InitializeControlEnumerator(Control ctrl)
    {
      if (null != ctrl)
      {
        m_enumerator = m_controls.GetEnumerator();
        m_enumerator.MoveNext();
        while (null != m_enumerator.Current.Value)
        {
          if (m_enumerator.Current.Value == ctrl)
            return;
          m_enumerator.MoveNext();
        }
      }
    }

    /// <summary>
    /// Returns the next control based on the TabIndex initialized
    /// in the member dictionary.
    /// </summary>
    private Control NextControl
    {
      get
      {
        var ctrl = GetActiveControl();
        if (m_enumerator.Current.Value != ctrl)
          InitializeControlEnumerator(ctrl);

        m_enumerator.MoveNext();

        if (null == m_enumerator.Current.Value)
        {
          m_enumerator = m_controls.GetEnumerator();
          m_enumerator.MoveNext();
        }

        return m_enumerator.Current.Value;
      }
    }

    /// <summary>
    /// Tab key pressed event handler
    /// </summary>
    private void OnKeyPress(object sender, KeyPressEventArgs e)
    {
      if (e.KeyChar == '\t')
      {
        NextControl.Focus();
      }
    }
  }
}
