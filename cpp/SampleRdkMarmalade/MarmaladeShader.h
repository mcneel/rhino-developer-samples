
#pragma once

class CMarmaladeParam
{
public:
	CMarmaladeParam() { }

	CMarmaladeParam(const CMarmaladeParam& p)
	              { m_sName = p.m_sName; m_sFriendlyName = p.m_sFriendlyName; m_vValue = p.m_vValue;
	                m_vMin = p.m_vMin; m_vMax = p.m_vMax; }

	CMarmaladeParam(const ON_wString& sName, const ON_wString& sFriendlyName, const CRhRdkVariant& vValue,
	                const CRhRdkVariant& vValueMin, const CRhRdkVariant& vValueMax)
	              { m_sName = sName; m_sFriendlyName = sFriendlyName; m_vValue = vValue;
	                m_vMin = vValueMin; m_vMax = vValueMax; }

	ON_wString m_sName;
	ON_wString m_sFriendlyName;
	CRhRdkVariant m_vValue;
	CRhRdkVariant m_vMin;
	CRhRdkVariant m_vMax;
};

class CMarmaladeParamBlock
{
public:
	virtual ~CMarmaladeParamBlock();

	void operator = (const CMarmaladeParamBlock& pb);

	void Add(CMarmaladeParam* pParam);
	int ParameterCount(void) const;
	CMarmaladeParam* Parameter(int iIndex) const;
	CMarmaladeParam* FindParameter(const ON_wString& sName) const;

private:
	ON_SimpleArray<CMarmaladeParam*> m_aParams;
};

class CMarmaladeShader
{
public:
	CMarmaladeShader();
	virtual ~CMarmaladeShader();

	virtual ON_wString Name(void) const = 0;
	virtual ON_wString FriendlyName(void) const = 0;
	virtual ON_wString Description(void) const = 0;
	virtual UUID Uuid(void) const = 0;
	virtual CMarmaladeShader* Clone(void) const = 0;

	CMarmaladeParamBlock& ParamBlock(void) { return m_ParamBlock; }
	const CMarmaladeParamBlock& ParamBlock(void) const { return m_ParamBlock; }

private:
	CMarmaladeParamBlock m_ParamBlock;
};

class CMarmaladeShaders
{
public:
	CMarmaladeShaders();
	~CMarmaladeShaders();

	int Count(void) const;
	const CMarmaladeShader* Shader(int iIndex) const;
	const CMarmaladeShader* FindShader(const UUID& uuid) const;

private:
	ON_SimpleArray<CMarmaladeShader*> m_aShaders;
};

extern CMarmaladeShaders& Shaders(void);
