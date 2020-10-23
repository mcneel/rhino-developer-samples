
#pragma once

class CContentTask : public CRhRdkTask
{
protected:
	virtual UUID PlugInId(void) const override;
	virtual UUID Id(void) const override { const UUID uuid = { 0xf7fb14a0, 0xb1fd, 0x4a1a, { 0xb6, 0xa8, 0x77, 0x57, 0x2c, 0xc5, 0xf9, 0xec } }; return uuid; }
	virtual bool IsEnabled(const IRhRdkTaskOrigin& origin) const override { return true; }
	virtual int MenuOrder(const IRhRdkTaskOrigin& origin) const override;
	virtual const wchar_t* MenuString(const IRhRdkTaskOrigin& origin, CRhRdkContent::Kinds kind) const override;
	virtual Result Execute(const IRhRdkTaskOrigin& origin) const override;
};
