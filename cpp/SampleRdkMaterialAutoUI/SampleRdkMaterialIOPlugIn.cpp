
#include "stdafx.h"
#include "SampleRdkMaterialIOPlugIn.h"
#include "SampleRdkMaterialAutoUIRdkPlugIn.h"
#include "SampleRdkMaterial.h"

/*	This example of a 'Content I/O plug-in' demonstrates how to save and load
	a render content from any desired file format. The example uses a file
	format with an extension of '.sample' and a simple name=value style grammar.

	Syntax
	------
	( ) denotes English description.
	{ } denotes zero or more occurrances.
	[ ] denotes optional items.
	' ' denotes a literal item.
	 |  denotes a choice ('OR').

	cr ::= (byte value 13)
	lf ::= (byte value 10)
	bool-value ::= 'true' | 'false'
	float-value ::= (any valid float literal, e.g., 3.1415926)
	string-value ::= '"' (any sequence of printable characters) '"'
	color-value ::= 'Color' '(' float-value ',' float-value ',' float-value ',' float-value ')'
	vec2d-value ::= 'Vector2D' '(' float-value ',' float-value ')'
	vec3d-value ::= 'Vector3D' '(' float-value ',' float-value ',' float-value ')'
	value ::= bool-value | float-value | string-value | color-value | vec2d-value | vec3d-value
	name ::= alpha | digit { alpha | digit }
	line ::= name '=' [ value ] [ cr ] lf
	sample-file ::= { line }

	Examples
	--------
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

CRhRdkContentKindList CSampleRdkMaterialIOPlugIn::SupportedKinds(void) const
{
	CRhRdkContentKindList list;
	list.Add(CRhRdkContent::Kinds::Material);

	return list;
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
	{
		p++;
	}
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
			s.Format(L"CSampleRdkMaterialIOPlugIn syntax error while loading file\n\n%ls\n\n"
			         L"Expected '%S' but found '%S' at line %d\n", static_cast<const wchar_t*>(m_sFilename), pSubString, p, m_iLine);
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
		value = false;
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
		SkipWhiteSpace(p);
		if (0 == *p)
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

static void SetMaterialProperty(CRhRdkBasicMaterial& bm, const ON_wString& sName, const CRhRdkVariant& vValue)
{
	bm.SetParameter(sName, vValue);
}

CRhRdkContent* CSampleRdkMaterialIOPlugIn::Load(const CRhinoDoc* pDoc, const wchar_t* wszFilename, CRhRdkContent::Kinds /*kind*/) const
{
	// Notes:
	//
	// - The document is only provided for completeness. You must NOT attach your loaded content to a document.
	//
	// - If this I/O plug-in only supports a single content kind, you should ignore 'kind' and create
	//   the content kind it supports. If this I/O plug-in supports multiple kinds, you MUST create
	//   the kind specified by 'kind'.

	m_sFilename = wszFilename;

	ON_Material mat;
	auto pMaterial = ::RhRdkNewBasicMaterial(mat, pDoc);
	pMaterial->Initialize();

	const ON_String sFilename = wszFilename;
	const auto pFile = fopen(sFilename, "rb");
	if (nullptr == pFile)
		return false;

	// Get the length of the file.
	fseek(pFile, 0, SEEK_END);
	const auto length = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	// Read the file into a buffer.
	char* pBuffer = new char[length+2];
	fread(pBuffer, sizeof(char), length, pFile);

	fclose(pFile);

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
				SetMaterialProperty(*pMaterial, sName, vValue);
			}

			// Search for end of line; stop if end of buffer.
			while ((0 != *p) && (('\r' == *p) || ('\n' == *p)))
				*p++;

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
			p++;
	}

	delete[] pBuffer;

	return pMaterial;
}

bool CSampleRdkMaterialIOPlugIn::Save(const wchar_t* wszFilename, const CRhRdkContent& c, const IRhRdkPreviewSceneServer*) const
{
	const ON_String sFilename = wszFilename;
	const auto pFile = fopen(sFilename, "wb");
	if (nullptr == pFile)
		return false;

	const auto& fields = c.Fields();

	for (int i = 0; i < fields.Count(); i++)
	{
		auto& field = *fields.Field(i);

		const wchar_t* wszName = field.InternalName();

		const wchar_t* wszValue = L"";
		const auto& vValue = field.Value();
		if (!vValue.IsNull())
			wszValue = vValue.AsString();

		ON_wString sText;

		switch (field.Value().Type())
		{
		case CRhRdkVariant::vtString:
			sText.Format(L"%ls = \"%ls\"\r\n", wszName, wszValue);
			break;

		case CRhRdkVariant::vtColor:
			sText.Format(L"%ls = %S(%ls)\r\n", wszName, szColor, wszValue);
			break;

		case CRhRdkVariant::vtVector2d:
			sText.Format(L"%ls = %S(%ls)\r\n", wszName, szVector2D, wszValue);
			break;

		case CRhRdkVariant::vtVector3d:
			sText.Format(L"%ls = %S(%ls)\r\n", wszName, szVector3D, wszValue);
			break;

		default:
			sText.Format(L"%ls = %ls\r\n", wszName, wszValue);
			break;
		}

		const ON_String s = sText;
		fwrite(s.Array(), sizeof(char), s.Length(), pFile);
	}

	fclose(pFile);

	return true;
}
