
#pragma once

#include "Resource.h"

class CSampleRdkMaterialCustomUIDlg : public CRhinoDialog, public CRhRdkCustomContentUI, protected CRhRdkEventWatcher
{
public:
	CSampleRdkMaterialCustomUIDlg(const UUID& uuidEditor, const UUID& uuidUI, CWnd* pParent);
	virtual ~CSampleRdkMaterialCustomUIDlg();

	void CreateDlg(CWnd* pParent);

	void OnColorButtonNotify(void);

private:
	enum { IDD = IDD_MATERIAL_SECTION_CUSTOM };
	CButton m_color_button_placeholder;
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
	afx_msg void OnKillfocusEditName();
	afx_msg void OnKillfocusEditNotes();
	DECLARE_MESSAGE_MAP()

protected:
	ON_SimpleArray<IRhinoUiDataSource*> m_aDS;
	IRhRdkColorButton* m_pColorButton = nullptr;
	int m_iInternalCall = 0;
};

class CColorButtonNotificationHandler : public IRhRdkNotificationHandler
{
public:
	CColorButtonNotificationHandler(CSampleRdkMaterialCustomUIDlg& s) : m_Section(s) { }

	virtual void DeleteThis(void) override { delete this; }
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) override { return nullptr; }
	virtual void OnBeginNotify(IRhinoUiWindow&, unsigned int) override { }
	virtual void OnEndNotify(IRhinoUiWindow&, unsigned int) override { }

	virtual void OnNotify(IRhinoUiWindow&, const CRhRdkVariant&, const CRhRdkVariant&, unsigned int) override
		{ m_Section.OnColorButtonNotify(); }

private:
	CSampleRdkMaterialCustomUIDlg& m_Section;
};
