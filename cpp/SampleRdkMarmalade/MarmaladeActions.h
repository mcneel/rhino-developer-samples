
#pragma once

class CMarmaladeExtraPeelAction : public CRhRdkAction
{
public:
	CMarmaladeExtraPeelAction(const UUID& uuidClient);

	static  UUID Ident(void);
	virtual UUID Uuid(void) const override { return Ident(); }
	virtual ON_wString Caption(void) const override { return L"Extra &Peel"; }
	virtual ON_wString ToolTip(void) const override { return L"Extra Peel"; }
	virtual bool Icon(const ON_2iSize& size, CRhinoDib& dibOut) const override { return false; } // TODO:
	virtual bool Execute(void) override;
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
	virtual UUID Uuid(void) const override { return Ident(); }
	virtual ON_wString Caption(void) const override { return L"New &Orange"; }
	virtual ON_wString ToolTip(void) const override { return L"New Orange"; }
	virtual bool Icon(const ON_2iSize& size, CRhinoDib& dibOut) const override;
	virtual bool Execute(void) override;
};

class CMarmaladeLemonAction : public CMarmaladeFlavorAction
{
public:
	CMarmaladeLemonAction(const UUID& uuidClient, const CMarmaladeExtraPeelAction& mpa);

	static  UUID Ident(void);
	virtual UUID Uuid(void) const override { return Ident(); }
	virtual ON_wString Caption(void) const override { return L"New &Lemon"; }
	virtual ON_wString ToolTip(void) const override { return L"New Lemon"; }
	virtual bool Icon(const ON_2iSize& size, CRhinoDib& dibOut) const override;
	virtual bool Execute(void) override;
};

class CMarmaladeLimeAction : public CMarmaladeFlavorAction
{
public:
	CMarmaladeLimeAction(const UUID& uuidClient, const CMarmaladeExtraPeelAction& mpa);

	static  UUID Ident(void);
	virtual UUID Uuid(void) const override { return Ident(); }
	virtual ON_wString Caption(void) const override { return L"New L&ime"; }
	virtual ON_wString ToolTip(void) const override { return L"New Lime"; }
	virtual bool Icon(const ON_2iSize& size, CRhinoDib& dibOut) const override;
	virtual bool Execute(void) override;
};

// Grouped (mutually exclusive) actions.

class CMarmaladeBreadAction : public CRhRdkAction
{
public:
	CMarmaladeBreadAction(const UUID& uuidClient);

	static  UUID Ident(void);
	virtual UUID Uuid(void) const override { return Ident(); }
	virtual ON_wString Caption(void) const override { return L"&Bread"; }
	virtual ON_wString ToolTip(void) const override { return L"Bread"; }
	virtual bool Icon(const ON_2iSize& size, CRhinoDib& dibOut) const override { return false; } // TODO:
	virtual int Group(void) const override { return 1; }
	virtual bool Execute(void) override { return false; }
};

class CMarmaladeToastAction : public CRhRdkAction
{
public:
	CMarmaladeToastAction(const UUID& uuidClient);

	static  UUID Ident(void);
	virtual UUID Uuid(void) const override { return Ident(); }
	virtual ON_wString Caption(void) const override { return L"&Toast"; }
	virtual ON_wString ToolTip(void) const override { return L"Toast"; }
	virtual bool Icon(const ON_2iSize& size, CRhinoDib& dibOut) const override { return false; } // TODO:
	virtual int Group(void) const override { return 1; }
	virtual bool Execute(void) override { return false; }
};

class CMarmaladeSconeAction : public CRhRdkAction
{
public:
	CMarmaladeSconeAction(const UUID& uuidClient);

	static  UUID Ident(void);
	virtual UUID Uuid(void) const override { return Ident(); }
	virtual ON_wString Caption(void) const override { return L"&Scone"; }
	virtual ON_wString ToolTip(void) const override { return L"Scone"; }
	virtual bool Icon(const ON_2iSize& size, CRhinoDib& dibOut) const override { return false; } // TODO:
	virtual int Group(void) const override { return 1; }
	virtual bool Execute(void) override { return false; }
};
