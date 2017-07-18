#include "Resource.h"

class CSampleAnimatorConduit : public CRhinoDisplayConduit
{
public:
  CSampleAnimatorConduit();
  bool ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate)  override;

public:
  ON_SimpleArray<const CRhinoObject*> m_objects;
  ON_Xform m_xform;
};


class CSampleAnimatorDialog : public CRhinoDialog
{
public:
  CSampleAnimatorDialog(CWnd* pParent, unsigned int document_sn);

  enum { IDD = IDD_ANIMATOR_DIALOG };
  CSliderCtrl	m_slider;

public:
  void RedrawDocument();

  int m_max_steps;
  ON_3dPoint m_start;
  ON_3dPointArray m_points;
  CSampleAnimatorConduit m_conduit;
  unsigned int m_document_sn;

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  virtual void OnOK() override;
  virtual void OnCancel() override;
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  DECLARE_MESSAGE_MAP()
};
