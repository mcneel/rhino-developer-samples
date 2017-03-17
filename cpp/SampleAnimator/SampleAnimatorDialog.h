/////////////////////////////////////////////////////////////////////////////
// SampleAnimatorDialog.h

#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CSampleAnimatorConduit declaration

class CSampleAnimatorConduit : public CRhinoDisplayConduit
{
public:
  CSampleAnimatorConduit();
  bool ExecConduit( CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate );

public:
  ON_SimpleArray<const CRhinoObject*> m_objects;
  ON_Xform m_xform;
};

/////////////////////////////////////////////////////////////////////////////
// CSampleAnimatorDialog declaration

class CSampleAnimatorDialog : public CRhinoDialog
{
public:
	CSampleAnimatorDialog( CWnd* pParent );

	enum { IDD = IDD_DIALOG };
	CSliderCtrl	m_slider;

public:
  void RedrawDocument();

  int m_max_steps;
  ON_3dPoint m_start;
  ON_3dPointArray m_points;
  CSampleAnimatorConduit m_conduit;

protected:
	virtual void DoDataExchange( CDataExchange* pDX );
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
	DECLARE_MESSAGE_MAP()
};
