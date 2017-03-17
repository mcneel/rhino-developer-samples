
#include "stdafx.h"
#include "SampleRdkMaterialIOPlugIn.h"
#include "SampleRdkMaterialAutoUIRdkPlugIn.h"
#include "SampleRdkMaterial.h"

/* .sample file format:

Type       Syntax
----       ------
<null>     <no text>
bool       <true|false>
float      <float>
String     "<string>"
Color      Color(<float-red>, <float-green>, <float-blue>, <float-alpha>)
Vector2D   Vector2D(<float-x>, <float-y>)
Vector3D   Vector3D(<float-x>, <float-y>, <float-z>)
 
Example:

	no-type =
	opaque = true
	gloss = 0.5
	text = "Fruit"
	color = Color(1.0, 0.5, 0.5, 1.0)
	2D-vector = Vector2D(0.2, 0.3)
	3D-vector = Vector3D(1, 2, 3)

*/

static const char* szColor    = "Color";
static const char* szVector2D = "Vector2D";
static const char* szVector3D = "Vector3D";

const wchar_t* CSampleRdkMaterialIOPlugIn::SupportedKinds(void) const
{
	// This I/O plug-in only supports a single content kind.
	return RDK_KIND_MATERIAL;
}

UUID CSampleRdkMaterialIOPlugIn::PlugInId(void) const
{
	return CSampleRdkMaterialAutoUIRdkPlugIn::RhinoPlugInId();
}

UUID CSampleRdkMaterialIOPlugIn::RdkPlugInId(void) const
{
	return CSampleRdkMaterialAutoUIRdkPlugIn::RdkPlugInId();
}

static _locale_t Locale(void)
{
	static _locale_t loc = nullptr;

	if (nullptr == loc)
	{
		loc = _create_locale(LC_ALL, "C");
	}

	return loc;
}

static void SkipWhiteSpace(char*& p)
{
	while ((' ' == *p) || ('\t' == *p))
		p++;
}

bool CSampleRdkMaterialIOPlugIn::Parse(char*& p, const char* pSubString, bool bExpect) const
{
	SkipWhiteSpace(p);

	char* pFound = strstr(p, pSubString);
	if ((nullptr == pFound) || (pFound != p))
	{
		if (bExpect)
		{
			ON_wString s;
			s.Format(L"CSampleRdkMaterialIOPlugIn syntax error while loading file\n\n%s\n\n"
			         L"Expected '%S' but found '%S' at line %d\n", (const wchar_t*)m_sFilename, pSubString, p, m_iLine);
			OutputDebugString(s);
			::RhRdkErrorBox(s);
		}

		return false;
	}

	p = pFound + strlen(pSubString);

	return true;
}

bool CSampleRdkMaterialIOPlugIn::ParseBool(char*& p, bool& value) const
{
	SkipWhiteSpace(p);

	if (Parse(p, "true", false))
	{
		value = true;
		return true;
	}
	else
	if (Parse(p, "false", true))
	{
		value = true;
		return true;
	}

	return false;
}

bool CSampleRdkMaterialIOPlugIn::ParseDouble(char*& p, double& value) const
{
	SkipWhiteSpace(p);

	value = _atof_l(p, Locale());

	while (('+' == *p) || ('-' == *p))
		*p++;

	if ('.' == *p)
		*p++;

	if (!isdigit(*p))
		return false;

	while (isdigit(*p))
		p++;

	if ('.' == *p)
		*p++;

	while (isdigit(*p))
		p++;

	bool bExponent = false;
	if ('e' == char(tolower(*p)))
	{
		bExponent = true;
		*p++;
	}

	while (('+' == *p) || ('-' == *p))
		*p++;

	if (bExponent && !isdigit(*p))
		return false;

	while (isdigit(*p))
		p++;

	return true;
}

bool CSampleRdkMaterialIOPlugIn::ParseFloat(char*& p, float& value) const
{
	double d = 0.0;
	const bool b = ParseDouble(p, d);
	value = float(d);

	return b;
}

bool CSampleRdkMaterialIOPlugIn::ParseFloatList(char*& p, float* a, int num) const
{
	for (int i  = 0; i < num; i++)
	{
		if (!ParseFloat(p, a[i]))
			return false;

		if ((i < (num - 1)) && !Parse(p, ","))
			return false;
	}

	return true;
}

CRhRdkVariant CSampleRdkMaterialIOPlugIn::GetValue(char* p) const
{
	CRhRdkVariant vValue;

	if (nullptr != p)
	{
		if (*p == 0)
		{
			// Null type; valid.
		}
		else
		if (Parse(p, "\"", false))
		{
			// String value.
			auto pStart = p;

			// Find end of string.
			while ((0 != *p) && ('"' != *p))
				p++;

			// Get string as variant.
			if (Parse(p, "\""))
			{
				*--p = 0;
				vValue = pStart;
				*p++ = '"';
			}
		}
		else
		if (Parse(p, szColor, false) && Parse(p, "("))
		{
			// Color value.
			float a[4];
			if (ParseFloatList(p, a, 4) && Parse(p, ")"))
			{
				vValue.SetValue(CRhRdkColor(a[0], a[1], a[2], a[3]));
			}
		}
		else
		if (Parse(p, szVector2D, false) && Parse(p, "("))
		{
			// 2D vector value.
			float a[2];
			if (ParseFloatList(p, a, 2) && Parse(p, ")"))
			{
				vValue.SetValue(ON_2dVector(a[0], a[1]));
			}
		}
		else
		if (Parse(p, szVector3D, false) && Parse(p, "("))
		{
			// 3D vector value.
			float a[3];
			if (ParseFloatList(p, a, 3) && Parse(p, ")"))
			{
				vValue.SetValue(ON_3dVector(a[0], a[1], a[2]));
			}
		}
		else
		{
			float f = 0.0f;
			if (ParseFloat(p, f))
			{
				vValue.SetValue(f);
			}
			else
			{
				bool b = false;
				if (ParseBool(p, b))
				{
					vValue.SetValue(b);
				}
			}
		}
	}

	return vValue;
}

CRhRdkContent* CSampleRdkMaterialIOPlugIn::Load(const CRhinoDoc*, const wchar_t* wszFilename, const wchar_t* /*wszKind*/) const
{
	// Notes:
	//
	// - The document is only provided for completeness. You must NOT attach your loaded content to a document.
	//
	// - If this I/O plug-in only supports a single content kind, you should ignore wszKind and create
	//   the content kind it supports. If this I/O plug-in supports multiple kinds, you MUST create
	//   the kind specified by wszKind.

	m_sFilename = wszFilename;

	auto pMaterial = new CSampleRdkMaterial;
	pMaterial->Initialize();

	CFile file;
	if (!file.Open(wszFilename, CFile::modeRead | CFile::typeBinary | CFile::shareDenyWrite))
		return false;

	const UINT length = UINT(file.GetLength());

	char* pBuffer = new char[length+2];
	file.Read(pBuffer, length);

	// Ensure end-of-line in buffer even if not in file.
	pBuffer[length+0] = '\n';

	// Terminate buffer.
	pBuffer[length+1] = 0;

	char* pLine = pBuffer;

	ON_wString sName;

	m_iLine = 1;

	char* p = pLine;
	char* pValue = nullptr;

	// Scan the text looking for '=' or end-of-line.
	// Stop at end of buffer.
	while (0 != *p)
	{
		const char c = *p;
		if ('=' == c)
		{
			// Terminate the name, copy it to sName and restore the buffer.
			*p = 0;
			sName = pLine;
			sName.TrimLeftAndRight();
			*p = c;

			// pValue points to start of value.
			pValue = p + 1;
		}
		else
		if (('\r' == c) || ('\n' == c))
		{
			// Reached the end of the line. Terminate the value,
			// get it as a variant, and restore the buffer.
			*p = 0;
			const CRhRdkVariant vValue = GetValue(pValue);
			*p = c;

			if (!sName.IsEmpty())
			{
				// Name is good so add a field to the material.
				pMaterial->AddField(sName, vValue);
			}

			// Search for end of line; stop if end of buffer.
			while ((0 != *p) && (('\r' == *p) || ('\n' == *p)))
			{
				*p++;
			}

			// If there are more characters to parse, set pLine to the start of the
			// next line and reset pValue to null in case the next line is invalid.
			if (0 != *p)
			{
				pLine = p--; // Retract p to compensate for the unavoidable p++.
				pValue = nullptr;
				m_iLine++;
			}
		}

		// If not at end of line, check the next character.
		if (0 != *p)
		{
			p++;
		}
	}

	file.Close();

	return pMaterial;
}

bool CSampleRdkMaterialIOPlugIn::Save(const wchar_t* wszFilename, const CRhRdkContent& c, const IRhRdkPreviewSceneServer*) const
{
	CFile file;
	if (!file.Open(wszFilename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareExclusive))
		return false;

	const auto& fields = c.Fields();

	for (int i = 0; i < fields.Count(); i++)
	{
		auto& field = *fields.Field(i);

		const wchar_t* wszName = field.InternalName();
		const auto wszValue = field.Value().AsString();

		ON_wString s;

		switch (field.Value().Type())
		{
		case CRhRdkVariant::vtString:
			s.Format(L"%s = \"%s\"\r\n", wszName, wszValue);
			break;

		case CRhRdkVariant::vtColor:
			s.Format(L"%s = %S(%s)\r\n", wszName, szColor, wszValue);
			break;

		case CRhRdkVariant::vtVector2d:
			s.Format(L"%s = %S(%s)\r\n", wszName, szVector2D, wszValue);
			break;

		case CRhRdkVariant::vtVector3d:
			s.Format(L"%s = %S(%s)\r\n", wszName, szVector3D, wszValue);
			break;

		default:
			s.Format(L"%s = %s\r\n", wszName, wszValue);
			break;
		}

		const int bufferLen = s.Length();
		char* pBuffer = new char[bufferLen];
		::WideCharToMultiByte(CP_ACP, 0, (const wchar_t*)s, -1, pBuffer, bufferLen, nullptr, nullptr);
		file.Write(pBuffer, bufferLen);
		delete[] pBuffer;
	}

	file.Close();

	return true;
}
