
#pragma once

#include "Resource.h"

class CSampleRdkMaterialCustomUIDlg : public CRhinoDialog, public CRhRdkCustomContentUI, protected CRhRdkEventWatcher
{
public:
	CSampleRdkMaterialCustomUIDlg(const UUID& uuidEditor, const UUID& uuidUI, CWnd* pParent);
	virtual ~CSampleRdkMaterialCustomUIDlg();

	void CreateDlg(CWnd* pParent);

private:
	enum { IDD = IDD_MATERIAL_SECTION_CUSTOM };
	CRhRdkColorButton m_color_button;
	CEdit m_edit_name;
	CEdit m_edit_notes;
	CButton m_check;

protected:
	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;

protected: // Implement IRhRdkContentUI.
	virtual void DeleteThis(void) override { delete this; }
	virtual void Destroy(void) override;
	virtual void SetPositionAndSize(const ON_4iRect& rect) override;
	virtual void DisplayData(void) override;
	virtual void EnableDisableControls(void) override { }
	virtual bool IsCreated(void) const override;
	virtual bool IsShown(void) const override;
	virtual void Show(bool bShow) override;
	virtual void Enable(bool bEnable) override;
	virtual RhRdkUiFrameworks Framework(void) const override { return RhRdkUiFrameworks::Win32; }
	virtual void* EVF(const wchar_t*, void*) override { return nullptr; }

protected: // Event watcher overrides.
	virtual void OnContentRenamed(const CRhRdkContent& c) override;
	virtual void OnContentNotesChanged(const CRhRdkContent& c) override;

protected:
	afx_msg LRESULT OnColorChanged(WPARAM w, LPARAM l);
	afx_msg void OnKillfocusEditName();
	afx_msg void OnKillfocusEditNotes();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

	void SetControlPositionAndSize(void);

protected:
	ON_SimpleArray<IRhinoUiDataSource*> m_aDS;
	CRhRdkControllerPtr m_Controller1;
	CRhRdkControllerPtr m_Controller2;
	int m_iInternalCall;
};
