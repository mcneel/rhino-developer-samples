#pragma once

#include "resource.h"

// CSampleDrawAnnotationConduit conduit

class CSampleDrawAnnotationConduit : public CRhinoDisplayConduit
{
public:
  CSampleDrawAnnotationConduit();
  bool ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate) override;

  void SetDimStyle(const ON_DimStyle& dim_style);
  void SetTextFont(const ON_Font& font);

private:
  ON_Text m_text;
  ON_DimStyle m_dim_style;
};

// CSampleDrawAnnotationDialog dialog

class CSampleDrawAnnotationDialog : public CRhinoDialog
{
	DECLARE_DYNAMIC(CSampleDrawAnnotationDialog)

public:
	CSampleDrawAnnotationDialog(CWnd* pParent, CRhinoDoc& doc);
  virtual ~CSampleDrawAnnotationDialog() = default;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FONT_DIALOG };
#endif
  CRhinoUiFontComboBox m_font_combo_box;
  CSampleDrawAnnotationConduit m_conduit;
  CRhinoDoc& m_doc;

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  afx_msg void OnClickedOk();
  afx_msg void OnClickedCancel();
  afx_msg void OnSelChangeComboFont();

	DECLARE_MESSAGE_MAP()
};
