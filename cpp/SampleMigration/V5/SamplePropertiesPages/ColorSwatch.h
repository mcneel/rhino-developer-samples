#pragma once


// CColorSwatch

class CColorSwatch : public CStatic
{
	DECLARE_DYNAMIC(CColorSwatch)

public:
	CColorSwatch();
	virtual ~CColorSwatch();

  COLORREF Color() const;
  void SetColor(COLORREF color);

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnPaint();

  void Draw(CDC& dc);

private:
  COLORREF m_color;

protected:
	DECLARE_MESSAGE_MAP()
};


