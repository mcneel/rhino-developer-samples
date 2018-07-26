#pragma once

// {68481907-79D9-4F9D-8F1C-62E553220DE2}
static const GUID SampleCustomRenderMeshUserDataUuid =
{ 0x68481907, 0x79d9, 0x4f9d,{ 0x8f, 0x1c, 0x62, 0xe5, 0x53, 0x22, 0xd, 0xe2 } };

class CSampleCustomRenderMeshUserData : public ON_UserData
{
	ON_OBJECT_DECLARE(CSampleCustomRenderMeshUserData);

public:

  CSampleCustomRenderMeshUserData();
  CSampleCustomRenderMeshUserData(const CSampleCustomRenderMeshUserData & src);

	virtual ~CSampleCustomRenderMeshUserData();

  CSampleCustomRenderMeshUserData & operator=(const CSampleCustomRenderMeshUserData & src);

  virtual bool GetDescription(ON_wString & description);

  virtual bool Archive() const;

	virtual bool Write(ON_BinaryArchive & binary_archive) const;
	virtual bool Read(ON_BinaryArchive & binary_archive);

	double Amount() const;
	void SetAmount(double amount);

protected:

  double m_amount;
};