
#pragma once

#include "MarmaladePlugIn.h"

/** In Rhino 6, custom 'actions' have been superseded by custom 'tasks'.

	CRhRdkAction is deprecated; use CRhRdkTask instead. The interface is a
	bit different and has more functions but the basic principle is the same.

*/

class CMarmaladeExtraPeelTask : public CRhRdkCustomTask // was CMarmaladeExtraPeelAction
{
public:
	bool IsChecked(void) const { return m_bChecked; }
	void SetIsChecked(bool b) { m_bChecked = b; }

	static  UUID Ident(void);
	virtual UUID Id(void) const override { return Ident(); }
	virtual const wchar_t* MenuString(const IRhRdkTaskOrigin&, CRhRdkContent::Kinds) const override { return L"Extra &Peel"; }
	virtual bool IconOut(CRhRdkContent::Kinds kind, int width, int height, OUT CRhinoDib& dib) const override { return false; }
	virtual bool IconIn(CRhRdkContent::Kinds kind, int width, int height, OUT CRhinoDib& dib) const override;
	virtual void Update(IRhRdkTaskUpdate& tu) const override;
	virtual Result Execute(const IRhRdkTaskOrigin& origin) const override;
	virtual UUID PlugInId(void) const override { return MarmaladePlugIn().PlugInID(); }
	virtual bool IsEnabled(const IRhRdkTaskOrigin& origin) const override { return true; }
	virtual int MenuOrder(const IRhRdkTaskOrigin& origin) const override { return 35; }
	virtual Separator MenuSeparators(void) const override { return Separator::Both; }

private:
	mutable bool m_bChecked = false;
};

class CMarmaladeFlavorTask : public CRhRdkCustomTask // was CMarmaladeFlavorAction
{
public:
	CMarmaladeFlavorTask(const CMarmaladeExtraPeelTask& mpa);

protected:
	void AddNewMaterial(COLORREF col1, COLORREF col2) const;

	virtual UUID PlugInId(void) const override { return MarmaladePlugIn().PlugInID(); }
	virtual bool IsEnabled(const IRhRdkTaskOrigin& origin) const override { return true; }

private:
	const CMarmaladeExtraPeelTask& m_ExtraPeelAction;
};

class CMarmaladeOrangeTask : public CMarmaladeFlavorTask // was CMarmaladeOrangeAction
{
public:
	CMarmaladeOrangeTask(const CMarmaladeExtraPeelTask& mpa);

	static  UUID Ident(void);
	virtual UUID Id(void) const override { return Ident(); }
	virtual UUID PlugInId(void) const override { return MarmaladePlugIn().PlugInID(); }
	virtual const wchar_t* MenuString(const IRhRdkTaskOrigin&, CRhRdkContent::Kinds) const override { return L"New &Orange"; }
	virtual bool IconOut(CRhRdkContent::Kinds kind, int width, int height, OUT CRhinoDib& dib) const override;
	virtual Result Execute(const IRhRdkTaskOrigin& origin) const override;
	virtual int MenuOrder(const IRhRdkTaskOrigin& origin) const override { return 10; }
};

class CMarmaladeLemonTask : public CMarmaladeFlavorTask // was CMarmaladeLemonAction
{
public:
	CMarmaladeLemonTask(const CMarmaladeExtraPeelTask& mpa);

	static  UUID Ident(void);
	virtual UUID Id(void) const override { return Ident(); }
	virtual const wchar_t* MenuString(const IRhRdkTaskOrigin&, CRhRdkContent::Kinds) const override { return L"New &Lemon"; }
	virtual bool IconOut(CRhRdkContent::Kinds kind, int width, int height, OUT CRhinoDib& dib) const override;
	virtual Result Execute(const IRhRdkTaskOrigin& origin) const override;
	virtual int MenuOrder(const IRhRdkTaskOrigin& origin) const override { return 20; }
};

class CMarmaladeLimeTask : public CMarmaladeFlavorTask // was CMarmaladeLimeAction
{
public:
	CMarmaladeLimeTask(const CMarmaladeExtraPeelTask& mpa);

	static  UUID Ident(void);
	virtual UUID Id(void) const override { return Ident(); }
	virtual const wchar_t* MenuString(const IRhRdkTaskOrigin&, CRhRdkContent::Kinds) const override { return L"New L&ime"; }
	virtual bool IconOut(CRhRdkContent::Kinds kind, int width, int height, OUT CRhinoDib& dib) const override;
	virtual Result Execute(const IRhRdkTaskOrigin& origin) const override;
	virtual int MenuOrder(const IRhRdkTaskOrigin& origin) const override { return 30; }
};

// Radio (mutually exclusive) actions.

class CMarmaladeBreadAction : public CRhRdkCustomTask
{
public:
	static  UUID Ident(void);
	virtual UUID Id(void) const override { return Ident(); }
	virtual UUID PlugInId(void) const override { return MarmaladePlugIn().PlugInID(); }
	virtual bool IsEnabled(const IRhRdkTaskOrigin& origin) const override { return true; }
	virtual const wchar_t* MenuString(const IRhRdkTaskOrigin&, CRhRdkContent::Kinds) const override { return L"&Bread"; }
	virtual bool IconOut(CRhRdkContent::Kinds kind, int width, int height, OUT CRhinoDib& dib) const override { return false; }
	virtual bool IconIn(CRhRdkContent::Kinds kind, int width, int height, OUT CRhinoDib& dib) const override;
	virtual void Update(IRhRdkTaskUpdate& tu) const override;
	virtual Result Execute(const IRhRdkTaskOrigin&) const override;
	virtual int MenuOrder(const IRhRdkTaskOrigin& origin) const override { return 40; }
};

class CMarmaladeToastAction : public CRhRdkCustomTask
{
public:
	static  UUID Ident(void);
	virtual UUID Id(void) const override { return Ident(); }
	virtual UUID PlugInId(void) const override { return MarmaladePlugIn().PlugInID(); }
	virtual bool IsEnabled(const IRhRdkTaskOrigin& origin) const override { return true; }
	virtual const wchar_t* MenuString(const IRhRdkTaskOrigin&, CRhRdkContent::Kinds) const override { return L"&Toast"; }
	virtual bool IconOut(CRhRdkContent::Kinds kind, int width, int height, OUT CRhinoDib& dib) const override { return false; }
	virtual bool IconIn(CRhRdkContent::Kinds kind, int width, int height, OUT CRhinoDib& dib) const override;
	virtual void Update(IRhRdkTaskUpdate& tu) const override;
	virtual Result Execute(const IRhRdkTaskOrigin&) const override;
	virtual int MenuOrder(const IRhRdkTaskOrigin& origin) const override { return 50; }
};

class CMarmaladeSconeAction : public CRhRdkCustomTask
{
public:
	static  UUID Ident(void);
	virtual UUID Id(void) const override { return Ident(); }
	virtual UUID PlugInId(void) const override { return MarmaladePlugIn().PlugInID(); }
	virtual bool IsEnabled(const IRhRdkTaskOrigin& origin) const override { return true; }
	virtual const wchar_t* MenuString(const IRhRdkTaskOrigin&, CRhRdkContent::Kinds) const override { return L"&Scone"; }
	virtual bool IconOut(CRhRdkContent::Kinds kind, int width, int height, OUT CRhinoDib& dib) const override { return false; }
	virtual bool IconIn(CRhRdkContent::Kinds kind, int width, int height, OUT CRhinoDib& dib) const override;
	virtual void Update(IRhRdkTaskUpdate& tu) const override;
	virtual Result Execute(const IRhRdkTaskOrigin&) const override;
	virtual int MenuOrder(const IRhRdkTaskOrigin& origin) const override { return 60; }
};
