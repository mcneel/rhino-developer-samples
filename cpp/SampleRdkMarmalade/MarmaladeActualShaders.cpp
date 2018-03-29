
#include "stdafx.h"
#include "MarmaladeActualShaders.h"

static const CRhRdkVariant vNull;

//---- CMarmaladeOrangeShader

CMarmaladeOrangeShader::CMarmaladeOrangeShader()
{
	ParamBlock().Add(new CMarmaladeParam(L"color",          L"Color", ON_Color(255, 140, 0), vNull, vNull));
	ParamBlock().Add(new CMarmaladeParam(L"transparency",   L"Transparency",   0.0, 0.0, 1.0));
	ParamBlock().Add(new CMarmaladeParam(L"ior",            L"IOR",            1.0, 0.0, vNull));
	ParamBlock().Add(new CMarmaladeParam(L"texture-amount", L"Texture Amount", 1.0, 0.0, 1.0));
	ParamBlock().Add(new CMarmaladeParam(L"texture-on",     L"Texture On", true, vNull, vNull));
}

UUID CMarmaladeOrangeShader::Uuid(void) const
{
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	static const UUID uuid =
	{
		// {DB390DE9-A0B3-4e5e-9D0B-536F6DD7E093}
		0xdb390de9, 0xa0b3, 0x4e5e, { 0x9d, 0xb, 0x53, 0x6f, 0x6d, 0xd7, 0xe0, 0x93 }
	};
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!

	return uuid;
}

//---- CMarmaladeSatinShader

CMarmaladeSatinShader::CMarmaladeSatinShader()
{
	ParamBlock().Add(new CMarmaladeParam(L"color",     L"Color", ON_Color(250, 250, 250), vNull, vNull));
	ParamBlock().Add(new CMarmaladeParam(L"silkiness", L"Silkiness", 0.8, 0.0, 1.0));
	ParamBlock().Add(new CMarmaladeParam(L"roughness", L"Roughness", 0.2, 0.0, 1.0));
}

UUID CMarmaladeSatinShader::Uuid(void) const
{
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	static const UUID uuid =
	{
		0xa58920af, 0xe8c5, 0x45fc, { 0xb9, 0x21, 0x6a, 0x6f, 0xe5, 0xc1, 0xfb, 0xb1 }
	};
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!

	return uuid;
}

//---- CMarmaladeLemonCurdShader

CMarmaladeLemonCurdShader::CMarmaladeLemonCurdShader()
{
	ParamBlock().Add(new CMarmaladeParam(L"color",        L"Color", ON_Color(190, 175, 40), vNull, vNull));
	ParamBlock().Add(new CMarmaladeParam(L"transparency", L"Transparency",  0.2, 0.0, 1.0));
	ParamBlock().Add(new CMarmaladeParam(L"graininess",   L"Graininess",    0.4, 0.0, 1.0));
	ParamBlock().Add(new CMarmaladeParam(L"bubbliness",   L"Bubbliness",    0.5, 0.0, 1.0));
}

UUID CMarmaladeLemonCurdShader::Uuid(void) const
{
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	static const UUID uuid =
	{
		0x3e969193, 0xe40e, 0x4ab8, { 0x8b, 0x37, 0xf, 0x48, 0x3a, 0x89, 0x39, 0xba }
	};
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!

	return uuid;
}
