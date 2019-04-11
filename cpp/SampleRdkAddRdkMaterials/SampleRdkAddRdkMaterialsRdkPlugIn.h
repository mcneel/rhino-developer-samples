
#pragma once

class CSampleRdkAddRdkMaterialsRdkPlugIn : public CRhRdkPlugIn
{
public:
	static UUID RhinoPlugInId(void);
	static UUID RdkPlugInId(void);

	virtual bool Initialize(void) override;
	virtual void Uninitialize(void) override;
	virtual CRhinoPlugIn& RhinoPlugIn(void) const override;
	virtual bool Icon(CRhinoDib& dibOut) const override;
	virtual UUID PlugInId(void) const override { return RdkPlugInId(); }
};
