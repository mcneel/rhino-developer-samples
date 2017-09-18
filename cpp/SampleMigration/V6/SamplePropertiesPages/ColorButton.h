#pragma once


// CColorButton

class CColorButton : public CButton
{
	DECLARE_DYNAMIC(CColorButton)

public:
	CColorButton();
	virtual ~CColorButton();

public:
  COLORREF Color() const;
  void SetColor(COLORREF color);

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnPaint();
	BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) override;

  void Draw(CDC& dc);
  virtual void ButtonClicked();

private:
  COLORREF m_color;

protected:
	DECLARE_MESSAGE_MAP()
};


