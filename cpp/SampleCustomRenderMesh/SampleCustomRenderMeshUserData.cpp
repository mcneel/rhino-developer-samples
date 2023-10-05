
#include "stdafx.h"
#include "SampleCustomRenderMeshPlugIn.h"
#include "SampleCustomRenderMeshUserData.h"

ON_OBJECT_IMPLEMENT(CSampleCustomRenderMeshUserData, ON_UserData, "CB36A3BC-9638-4637-8617-2386705CB036");

CSampleCustomRenderMeshUserData::CSampleCustomRenderMeshUserData()
{
	m_application_uuid = SampleCustomRenderMeshPlugIn().PlugInID();
	m_userdata_uuid = SampleCustomRenderMeshUserDataUuid;
	m_userdata_copycount = 1;
}

CSampleCustomRenderMeshUserData::CSampleCustomRenderMeshUserData(const CSampleCustomRenderMeshUserData& src)
: ON_UserData(src)
{
	m_userdata_copycount = src.m_userdata_copycount;
	m_amount = src.m_amount;
}

CSampleCustomRenderMeshUserData::~CSampleCustomRenderMeshUserData()
{
}

CSampleCustomRenderMeshUserData & CSampleCustomRenderMeshUserData::operator=(const CSampleCustomRenderMeshUserData & src)
{
	ON_UserData::operator = (src);
	m_amount = src.m_amount;
	return *this;
}

bool CSampleCustomRenderMeshUserData::GetDescription(ON_wString & description)
{
	description = L"SampleCustomRenderMeshUserData";
	return true;
}

double CSampleCustomRenderMeshUserData::Amount() const
{
  return m_amount;
}

void CSampleCustomRenderMeshUserData::SetAmount(double amount)
{
  m_amount = amount;
}

bool CSampleCustomRenderMeshUserData::Archive() const
{
	// Save this user data to the archive with object.
	return true;
}

bool CSampleCustomRenderMeshUserData::Write(ON_BinaryArchive & archive) const
{
	// Write user data version first. Later there may be a need to extend this user data.
	const int version = 1;

	if (!archive.WriteInt(version))
		return false;

	// Version 1 only has amount as a double.
	if (!archive.WriteDouble(m_amount))
		return false;

	return true;
}

bool CSampleCustomRenderMeshUserData::Read(ON_BinaryArchive& archive)
{
	int version = 1;

	if (!archive.ReadInt(&version))
		return false;

	if (version == 1)
	{
		// Version 1 user data only has the amount as a double.
		if (!archive.ReadDouble(&m_amount))
			return false;
	}
	else
	{
		// User data in the archive is not supported by this version of the plug-in.
		return false;
	}

	return true;
}
