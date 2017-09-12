#pragma once

class CSampleMessageOnlyWnd : public CWnd
{
  DECLARE_DYNAMIC(CSampleMessageOnlyWnd)

public:
  CSampleMessageOnlyWnd();
  virtual ~CSampleMessageOnlyWnd();

public:
  // Create the message-only window
  static bool CreateWnd();

  // Destroy the message-only window
  static void DestroyWnd();

protected:
  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
  DECLARE_MESSAGE_MAP()

private:
  static CSampleMessageOnlyWnd* m_pWnd;
};


