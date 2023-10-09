
#pragma once

static const UUID SampleCustomRenderMeshUserDataUuid =
{
	0x68481907, 0x79d9, 0x4f9d,{ 0x8f, 0x1c, 0x62, 0xe5, 0x53, 0x22, 0xd, 0xe2 }
};

class CSampleCustomRenderMeshUserData : public ON_UserData
{
	ON_OBJECT_DECLARE(CSampleCustomRenderMeshUserData);
public:
	CSampleCustomRenderMeshUserData();
	CSampleCustomRenderMeshUserData(const CSampleCustomRenderMeshUserData& src);
	virtual ~CSampleCustomRenderMeshUserData();

	double Amount() const;
	void SetAmount(double amount);

	CSampleCustomRenderMeshUserData& operator = (const CSampleCustomRenderMeshUserData& src);

	virtual bool GetDescription(ON_wString& description) override;
	virtual bool Archive() const override;
	virtual bool Write(ON_BinaryArchive& binary_archive) const override;
	virtual bool Read(ON_BinaryArchive& binary_archive) override;

protected:
	double m_amount = 0.0;
};
