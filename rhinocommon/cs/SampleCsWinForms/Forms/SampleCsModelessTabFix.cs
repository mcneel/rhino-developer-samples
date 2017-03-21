// ***********************************************************************
// ModelessTabFix class
//
// Robert McNeel & Associates
// John Morse, October 2012
//
// C# Implementation of the following MSDN article:
//   http://support.microsoft.com/kb/233263
// ***********************************************************************

using System;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace SampleCsWinForms.Forms
{
  /// <summary>
  /// Simple class that use the WH_GETMESSAGE hook to capture the keystroke
  /// messages and call the IsDialogMessage API. If IsDialogMessage returns
  /// TRUE, then do not pass the message on to the message pump. The hook is
  /// set when the controls window is created and unset when the control
  /// window is destroyed.
  /// 
  /// See http://support.microsoft.com/kb/233263 for details.
  /// 
  /// Example usage:
  /// </summary>
  class ModelessTabFix
  {
    #region Private members
    private LocalWindowsHook m_LocalWindowsHook = new LocalWindowsHook(HookType.WH_GETMESSAGE);
    private Control m_Control;
    #endregion Private members

    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="control">Control to hook</param>
    public ModelessTabFix(Control control)
    {
      m_Control = control;
      m_Control.HandleCreated += ControlHandleCreated;
      m_Control.HandleDestroyed += ControlHandleDestroyed;
      m_LocalWindowsHook.HookInvoked += LocalWindowsHook_HookInvoked;
    }

    #region Control event callbacks
    /// <summary>
    /// Install the hook when the control window handle is created
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    void ControlHandleCreated(object sender, EventArgs e)
    {
      m_LocalWindowsHook.Install();
    }
    /// <summary>
    /// Uninstall the hook when the control window is destroyed
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    void ControlHandleDestroyed(object sender, EventArgs e)
    {
      m_LocalWindowsHook.Uninstall();
    }
    #endregion Control event callbacks

    /// <summary>
    /// LocalWindowsHook.HookInvoked event callback, called by LocalWindowsHook
    /// when a message is received.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    void LocalWindowsHook_HookInvoked(object sender, HookEventArgs e)
    {
      if (e.HookCode >= 0 && new IntPtr(WindowMessage.PM_REMOVE) == e.wParam)
      {
        // Copy the message to a local int variable
        var msg = (uint)Marshal.ReadInt32(e.lParam, Marshal.OffsetOf(typeof(WindowsInterop.MSG), "Msg").ToInt32());
        // Don't translate non-input events.
        if (msg >= WindowMessage.WM_KEYFIRST && msg <= WindowMessage.WM_KEYLAST)
        {
          if (WindowsInterop.IsDialogMessage(m_Control.Handle, e.lParam) != 0)
          {
            // The value returned from this hook is ignored, 
            // and it cannot be used to tell Windows the message has been handled.
            // To avoid further processing, convert the message to WM_NULL 
            // before returning.
            var type = typeof (WindowsInterop.MSG);
            Marshal.WriteInt32(e.lParam, Marshal.OffsetOf(type, "Msg").ToInt32(), WindowMessage.WM_NULL);
            Marshal.WriteInt32(e.lParam,Marshal.OffsetOf(type, "wParam").ToInt32(), 0);
            Marshal.WriteInt32(e.lParam, Marshal.OffsetOf(type, "lParam").ToInt32(), 0);
          }
        }
      }
    }

    /// <summary>
    /// Windows message Id constants
    /// </summary>
    class WindowMessage
    {
      public const int WM_NULL = 0;
      public const int WM_KEYFIRST = 0x0100;
      public const int WM_KEYLAST = 0x0109;
      public const int PM_REMOVE = 0x0001;
    }

    /// <summary>
    /// Windows imports
    /// </summary>
    static class WindowsInterop
    {
      /// <summary>
      /// Windows MSG structure definition
      /// </summary>
      [StructLayout(LayoutKind.Sequential)]
      public struct MSG
      {
        IntPtr hwnd;
        uint Msg;
        IntPtr wParam;
        IntPtr lParam;
        int time;
        //POINT pt
        int x;
        int y;
      }

      /// <summary>
      /// See: http://support.microsoft.com/kb/71450 
      /// </summary>
      /// <param name="hDlg"></param>
      /// <param name="lpMsg"></param>
      /// <returns></returns>
      [DllImport("user32.dll")]
      public static extern int IsDialogMessage(IntPtr hDlg, IntPtr lpMsg);
    }
  }
}
