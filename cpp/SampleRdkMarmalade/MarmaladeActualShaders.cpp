
#include "stdafx.h"
#include "MarmaladeActualShaders.h"

static const CRhRdkVariant vNull;

//---- CMarmaladeOrangeShader

CMarmaladeOrangeShader::CMarmaladeOrangeShader()
{
	ParamBlock().Add(new CMarmaladeParam(MARM_SHADER_COLOR,          L"Color", ON_Color(255, 140, 0), vNull, vNull));
	ParamBlock().Add(new CMarmaladeParam(MARM_SHADER_TRANSPARENCY,   L"Transparency",   0.0, 0.0, 1.0));
	ParamBlock().Add(new CMarmaladeParam(MARM_SHADER_IOR,            L"IOR",            1.0, 0.0, vNull));
	ParamBlock().Add(new CMarmaladeParam(MARM_SHADER_TEXTURE_ON,     L"Texture On",  true, vNull, vNull));
	ParamBlock().Add(new CMarmaladeParam(MARM_SHADER_TEXTURE_AMOUNT, L"Texture Amount", 1.0, 0.0, 1.0));
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
	ParamBlock().Add(new CMarmaladeParam(MARM_SHADER_COLOR,     L"Color", ON_Color(250, 250, 250), vNull, vNull));
	ParamBlock().Add(new CMarmaladeParam(MARM_SHADER_SILKINESS, L"Silkiness", 0.8, 0.0, 1.0));
	ParamBlock().Add(new CMarmaladeParam(MARM_SHADER_ROUGHNESS, L"Roughness", 0.2, 0.0, 1.0));
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
	ParamBlock().Add(new CMarmaladeParam(MARM_SHADER_COLOR,        L"Color", ON_Color(190, 175, 40), vNull, vNull));
	ParamBlock().Add(new CMarmaladeParam(MARM_SHADER_TRANSPARENCY, L"Transparency",  0.2, 0.0, 1.0));
	ParamBlock().Add(new CMarmaladeParam(MARM_SHADER_GRAININESS,   L"Graininess",    0.4, 0.0, 1.0));
	ParamBlock().Add(new CMarmaladeParam(MARM_SHADER_BUBBLINESS,   L"Bubbliness",    0.5, 0.0, 1.0));
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
