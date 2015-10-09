// ***********************************************************************
//  LocalWindowsHook class
//  Dino Esposito, summer 2002
// 
//  Provide a general infrastructure for using Win32 
//  hooks in .NET applications
// 
// The article:
// http://msdn.microsoft.com/en-us/magazine/cc188966.aspx
// The original WindowsHook.cs file
// http://msdn.microsoft.com/en-us/magazine/cc164959.aspx?code=true&level=root%2cCuttingEdge_1002%2cC%23%2cWindowsHook&file=WindowsHook.cs
// 
// This class was slightly modified to get rid of a compiler warning, see
// the "Add my McNeel" section at the end of the file.
// ***********************************************************************


using System;
using System.Runtime.InteropServices;

namespace WinHooks
{
  #region Class HookEventArgs
  class HookEventArgs : EventArgs
  {
    public int HookCode;	// Hook code
    public IntPtr wParam;	// WPARAM argument
    public IntPtr lParam;	// LPARAM argument
  }
  #endregion

  #region Enum HookType
  // Hook Types
  internal enum HookType : int
  {
    WH_JOURNALRECORD = 0,
    WH_JOURNALPLAYBACK = 1,
    WH_KEYBOARD = 2,
    WH_GETMESSAGE = 3,
    WH_CALLWNDPROC = 4,
    WH_CBT = 5,
    WH_SYSMSGFILTER = 6,
    WH_MOUSE = 7,
    WH_HARDWARE = 8,
    WH_DEBUG = 9,
    WH_SHELL = 10,
    WH_FOREGROUNDIDLE = 11,
    WH_CALLWNDPROCRET = 12,
    WH_KEYBOARD_LL = 13,
    WH_MOUSE_LL = 14
  }
  #endregion

  #region Class LocalWindowsHook
  class LocalWindowsHook
  {
    // ************************************************************************
    // Filter function delegate
    public delegate int HookProc(int code, IntPtr wParam, IntPtr lParam);
    // ************************************************************************

    // ************************************************************************
    // Internal properties
    protected IntPtr m_hhook = IntPtr.Zero;
    protected HookProc m_filterFunc = null;
    protected HookType m_hookType;
    // ************************************************************************

    // ************************************************************************
    // Event delegate
    public delegate void HookEventHandler(object sender, HookEventArgs e);
    // ************************************************************************

    // ************************************************************************
    // Event: HookInvoked 
    public event HookEventHandler HookInvoked;
    protected void OnHookInvoked(HookEventArgs e)
    {
      if (HookInvoked != null)
        HookInvoked(this, e);
    }
    // ************************************************************************

    // ************************************************************************
    // Class constructor(s)
    public LocalWindowsHook(HookType hook)
    {
      m_hookType = hook;
      m_filterFunc = new HookProc(this.CoreHookProc);
    }
    public LocalWindowsHook(HookType hook, HookProc func)
    {
      m_hookType = hook;
      m_filterFunc = func;
    }
    // ************************************************************************

    // ************************************************************************
    // Default filter function
    protected int CoreHookProc(int code, IntPtr wParam, IntPtr lParam)
    {
      if (code < 0)
        return CallNextHookEx(m_hhook, code, wParam, lParam);

      // Let clients determine what to do
      HookEventArgs e = new HookEventArgs();
      e.HookCode = code;
      e.wParam = wParam;
      e.lParam = lParam;
      OnHookInvoked(e);

      // Yield to the next hook in the chain
      return CallNextHookEx(m_hhook, code, wParam, lParam);
    }
    // ************************************************************************

    // ************************************************************************
    // Install the hook
    public void Install()
    {
      m_hhook = SetWindowsHookEx(m_hookType, m_filterFunc, IntPtr.Zero, GetCurrentThreadId());
    }
    // ************************************************************************

    // ************************************************************************
    // Uninstall the hook
    public void Uninstall()
    {
      UnhookWindowsHookEx(m_hhook);
    }
    // ************************************************************************


    #region Win32 Imports
    // ************************************************************************
    // Win32: SetWindowsHookEx()
    [DllImport("user32.dll")]
    protected static extern IntPtr SetWindowsHookEx(HookType code,
      HookProc func,
      IntPtr hInstance,
      int threadID);
    // ************************************************************************

    // ************************************************************************
    // Win32: UnhookWindowsHookEx()
    [DllImport("user32.dll")]
    protected static extern int UnhookWindowsHookEx(IntPtr hhook);
    // ************************************************************************

    // ************************************************************************
    // Win32: CallNextHookEx()
    [DllImport("user32.dll")]
    protected static extern int CallNextHookEx(IntPtr hhook, int code, IntPtr wParam, IntPtr lParam);
    // ************************************************************************
    #endregion

    #region Add my McNeel
    // ************************************************************************
    // Win32: GetCurrentThreadId()
    [DllImport("kernel32.dll")]
    protected static extern int GetCurrentThreadId();
    // ************************************************************************
    #endregion
  }
  #endregion
}