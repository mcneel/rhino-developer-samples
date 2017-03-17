
#pragma once

class CMarmaladeExtraPeelAction : public CRhRdkAction
{
public:
	CMarmaladeExtraPeelAction(const UUID& uuidClient);

	static  UUID Ident(void);
	virtual UUID Uuid(void) const { return Ident(); }
	virtual ON_wString Caption(void) const { return L"Extra &Peel"; }
	virtual ON_wString ToolTip(void) const { return L"Extra Peel"; }
	virtual HICON Icon(void) const { return NULL ;}
	virtual bool Execute(void);
};

class CMarmaladeFlavorAction : public CRhRdkAction
{
public:
	CMarmaladeFlavorAction(const UUID& uuidClient, const CMarmaladeExtraPeelAction& mpa);

protected:
	void AddNewMaterial(COLORREF col1, COLORREF col2) const;

private:
	const CMarmaladeExtraPeelAction& m_ExtraPeelAction;
};

class CMarmaladeOrangeAction : public CMarmaladeFlavorAction
{
public:
	CMarmaladeOrangeAction(const UUID& uuidClient, const CMarmaladeExtraPeelAction& mpa);

	static  UUID Ident(void);
	virtual UUID Uuid(void) const { return Ident(); }
	virtual ON_wString Caption(void) const { return L"New &Orange"; }
	virtual ON_wString ToolTip(void) const { return L"New Orange"; }
	virtual HICON Icon(void) const;
	virtual bool Execute(void);
};

class CMarmaladeLemonAction : public CMarmaladeFlavorAction
{
public:
	CMarmaladeLemonAction(const UUID& uuidClient, const CMarmaladeExtraPeelAction& mpa);

	static  UUID Ident(void);
	virtual UUID Uuid(void) const { return Ident(); }
	virtual ON_wString Caption(void) const { return L"New &Lemon"; }
	virtual ON_wString ToolTip(void) const { return L"New Lemon"; }
	virtual HICON Icon(void) const;
	virtual bool Execute(void);
};

class CMarmaladeLimeAction : public CMarmaladeFlavorAction
{
public:
	CMarmaladeLimeAction(const UUID& uuidClient, const CMarmaladeExtraPeelAction& mpa);

	static  UUID Ident(void);
	virtual UUID Uuid(void) const { return Ident(); }
	virtual ON_wString Caption(void) const { return L"New L&ime"; }
	virtual ON_wString ToolTip(void) const { return L"New Lime"; }
	virtual HICON Icon(void) const;
	virtual bool Execute(void);
};

// Grouped (mutually exclusive) actions.

class CMarmaladeBreadAction : public CRhRdkAction
{
public:
	CMarmaladeBreadAction(const UUID& uuidClient);

	static  UUID Ident(void);
	virtual UUID Uuid(void) const { return Ident(); }
	virtual ON_wString Caption(void) const { return L"&Bread"; }
	virtual ON_wString ToolTip(void) const { return L"Bread"; }
	virtual HICON Icon(void) const { return NULL; }
	virtual int Group(void) const { return 1; }
	virtual bool Execute(void) { return false; }
};

class CMarmaladeToastAction : public CRhRdkAction
{
public:
	CMarmaladeToastAction(const UUID& uuidClient);

	static  UUID Ident(void);
	virtual UUID Uuid(void) const { return Ident(); }
	virtual ON_wString Caption(void) const { return L"&Toast"; }
	virtual ON_wString ToolTip(void) const { return L"Toast"; }
	virtual HICON Icon(void) const { return NULL; }
	virtual int Group(void) const { return 1; }
	virtual bool Execute(void) { return false; }
};

class CMarmaladeSconeAction : public CRhRdkAction
{
public:
	CMarmaladeSconeAction(const UUID& uuidClient);

	static  UUID Ident(void);
	virtual UUID Uuid(void) const { return Ident(); }
	virtual ON_wString Caption(void) const { return L"&Scone"; }
	virtual ON_wString ToolTip(void) const { return L"Scone"; }
	virtual HICON Icon(void) const { return NULL; }
	virtual int Group(void) const { return 1; }
	virtual bool Execute(void) { return false; }
};
