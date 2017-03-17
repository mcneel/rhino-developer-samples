
#include "stdafx.h"
#include "MarmaladeShader.h"
#include "MarmaladeActualShaders.h"

//---- CMarmaladeParamBlock

CMarmaladeParamBlock::~CMarmaladeParamBlock()
{
	for (int i = 0; i < m_aParams.Count(); i++)
	{
		delete m_aParams[i];
	}
}

void CMarmaladeParamBlock::operator = (const CMarmaladeParamBlock& pb)
{
	for (int i = 0; i < pb.m_aParams.Count(); i++)
	{
		const CMarmaladeParam& param = *pb.Parameter(i);
		m_aParams.Append(new CMarmaladeParam(param));
	}
}

void CMarmaladeParamBlock::Add(CMarmaladeParam* pParam)
{
	m_aParams.Append(pParam);
}

int CMarmaladeParamBlock::ParameterCount(void) const
{
	return m_aParams.Count();
}

CMarmaladeParam* CMarmaladeParamBlock::Parameter(int iIndex) const
{
	if ((iIndex < 0) || (iIndex >= m_aParams.Count()))
		return NULL;

	return m_aParams[iIndex];
}

CMarmaladeParam* CMarmaladeParamBlock::FindParameter(const ON_wString& sName) const
{
	for (int i = 0; i < m_aParams.Count(); i++)
	{
		if (0 == m_aParams[i]->m_sName.CompareNoCase(sName))
			return m_aParams[i];
	}

	return NULL;
}

//---- CMarmaladeShader

CMarmaladeShader::CMarmaladeShader(void)
{
}

CMarmaladeShader::~CMarmaladeShader(void)
{
}

//---- CMarmaladeShaders -- Collection of shaders.

CMarmaladeShaders::CMarmaladeShaders()
{
	m_aShaders.Append(new CMarmaladeOrangeShader);
	m_aShaders.Append(new CMarmaladeSatinShader);
	m_aShaders.Append(new CMarmaladeLemonCurdShader);
}

CMarmaladeShaders::~CMarmaladeShaders()
{
	for (int i = 0; i < m_aShaders.Count(); i++)
	{
		delete m_aShaders[i];
	}
}

const CMarmaladeShader* CMarmaladeShaders::FindShader(const UUID& uuid) const
{
	for (int i = 0; i < m_aShaders.Count(); i++)
	{
		if (m_aShaders[i]->Uuid() == uuid)
			return m_aShaders[i];
	}

	return NULL;
}

int CMarmaladeShaders::Count(void) const
{
	return m_aShaders.Count();
}

const CMarmaladeShader* CMarmaladeShaders::Shader(int iIndex) const
{
	if ((iIndex < 0) || (iIndex >= m_aShaders.Count()))
		return NULL;

	return m_aShaders[iIndex];
}

CMarmaladeShaders& Shaders(void)
{
	static CMarmaladeShaders s;

	return s;
}
