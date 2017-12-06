#include "stdafx.h"
#include "resource.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleWithLicensingValidator license validator
//

class CSampleWithLicensingValidator : public CRhinoLicenseValidator
{
public:
  CSampleWithLicensingValidator();
  ~CSampleWithLicensingValidator() = default;

  // Returns the 'build' type of this plug-in
  CRhinoLicenseValidator::product_build_type ProductBuildType() override;
  
  // Obsolete
  CRhinoLicenseValidator::result ValidateProductKey(const wchar_t* product_key) override;
  
  // This member is called by Rhino, from CRhinoPlugIn::GetLicense(), when it
  // needs your plug-in to validate your product key or license.
  CRhinoLicenseValidator::result VerifyLicenseKey(
    const wchar_t* licenseKey, 
    const wchar_t* validationCode, 
    const double validationCodeInstalledDate, 
    bool gracePeriodExpired
  ) override;
  
  // Called by Rhino, from CRhinoPlugIn::GetLicense(), after a call to VerifyLicenseKey that
  // sets m_requires_previous_version_license_verification to true.
  bool VerifyPreviousVersionLicense(
    const wchar_t* license, 
    const wchar_t* previousVersionLicense
  ) override;

  // When Rhino Accounts gets a new lease, this function is called.
  void OnLeaseChanged(CRhinoLeaseChangedEventArgs&) override;

private:
  ON_wString Reverse(const wchar_t* input);
};

// The one and only CSampleWithLicensingValidator object
static class CSampleWithLicensingValidator theSampleWithLicensingValidator;

CSampleWithLicensingValidator::CSampleWithLicensingValidator()
{
  m_requires_online_validation = true;
}

CRhinoLicenseValidator::product_build_type CSampleWithLicensingValidator::ProductBuildType()
{
  return CRhinoLicenseValidator::release_build;
}

CRhinoLicenseValidator::result CSampleWithLicensingValidator::ValidateProductKey(const wchar_t* product_key)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  if (nullptr == product_key || 0 == product_key[0])
    return CSampleWithLicensingValidator::error_show_message;

  // This value will never be display in any user interface.
  // When your plugin's ValidateProductKey function is called, it is
  // passed a a product, or CD, key that was entered into the Zoo
  // administrator console. Your ValidateProductKey function will validate
  // the product key and decode it into a product license. This is
  // where you can store this license. This value will be passed
  // to your application at runtime when it requests a license.
  m_product_license = product_key;

  // This value will display in user interface items, such as in
  // the Zoo console and in About dialog boxes. Also, this value
  // is used to uniquely identify this license. Thus, it is
  // critical that this value be unique per product key, entered
  // by the administrator. No other license of this product, as
  // validated by this plugin, should return this value.
  //
  // This example just reverses product_key...
  m_serial_number = Reverse(product_key);

  // This value will display in user interface items, such as in
  // the Zoo console and in About dialog boxes.
  // (e.g. "Rhinoceros 6", "Rhinoceros 6 Commercial", etc.)
  m_license_title = "SampleWithLicensing 1.0 Commercial";

  // The build of the product that this license work with.
  // When your product requests a license from the Zoo, it
  // will specify one of these build types. Note, this is just
  // the int version of CRhinoLicenseValidator::product_build_type
  // enum value.
  m_build_type = CRhinoLicenseValidator::release_build;

  // Zoo licenses can be used by more than one instance of any application.
  // For example, a single Rhino Education Lab license can be used by up
  // to 30 systems simultaneously. If your license supports multiple instance,
  // then specify the number of supported instances here. Otherwise just
  // specify a value of 1 for single instance use.
  m_license_count = 1;

  // The Zoo supports licenses that expire. If your licensing scheme
  // is sophisticated enough to support this, then specify the
  // expiration date here. Note, this value must be specified in
  // Coordinated Universal Time (UTC). If your license does not expire,
  // then just this value to null.
  memset(&m_date_to_expire, 0, sizeof(m_date_to_expire));

  // This icon will displayed in the "Licenses" page in the Options dialog.
  // Note, Rhino will make a copy of this icon..
  m_product_icon =  CRhinoDpi::LoadIcon(AfxGetInstanceHandle(), IDI_MAIN, 32, 32);

  return CRhinoLicenseValidator::success;
}

CRhinoLicenseValidator::result  CSampleWithLicensingValidator::VerifyLicenseKey(
  const wchar_t* licenseKey, 
  const wchar_t* validationCode, 
  const double validationCodeInstalledDate, 
  bool gracePeriodExpired
)
{
  CString sLicenseKey(licenseKey);
  CString sValidationCode(validationCode);
  m_requires_online_validation = false;
  m_requires_previous_version_license_verification = false;
  if (sLicenseKey.IsEmpty())
  {
    sLicenseKey = "evaluate-111-222";
  }

  if (sLicenseKey.Left(8).CompareNoCase(L"evaluate-") == 0)
  {
    // Hard-coded evaluation licenses don't get validated online
    m_requires_online_validation = false;
  }

  m_product_license = sLicenseKey;
  m_serial_number = sLicenseKey.MakeReverse();
  m_license_title = L"Sample License";
  m_license_count = 1;

  // This license expires in 90 days.
  COleDateTime expire_date = COleDateTime::GetCurrentTime();
  COleDateTimeSpan interval;
  interval.SetDateTimeSpan(90, 0, 0, 0);
  m_date_to_expire = expire_date + interval;

  if (!sValidationCode.IsEmpty())
  {
    if (sValidationCode.CompareNoCase(L"invalid") == 0)
    {
      m_error_message = L"Your validation code is invalid";
      return CRhinoLicenseValidator::error_show_message;
    }
  }

  if (sLicenseKey.Left(8).CompareNoCase(L"upgrade-") == 0)
  {
    m_requires_previous_version_license_verification = true;
  }

  return CRhinoLicenseValidator::success;
}

bool CSampleWithLicensingValidator::VerifyPreviousVersionLicense(
  const wchar_t* license, 
  const wchar_t* previousVersionLicense
)
{
  CString sLicense(license);
  CString sPrevVersionLicense(previousVersionLicense);

  if (sLicense.IsEmpty())
    return false;
  if (sPrevVersionLicense.IsEmpty())
    return false;

  // For this example, we'll consider any non-empty previous version license to be valid.
  return true;
}

void CSampleWithLicensingValidator::OnLeaseChanged(CRhinoLeaseChangedEventArgs& args)
{
  CRhinoLicenseLease* pLease = args.GetLease();
  if (pLease == nullptr)
  {
    // Lease has been voided; this product should behave as if it has no
    // license. It is up to the plug-in to determine what that looks like.
    return;
  }

  // Verify that pLease->ProductId is correct
  // Verify that pLease->ProductEdition is correct
  // Verify that pLease->ProductVersion is correct
  // Verify that pLease->IsExpired() is false
}

ON_wString CSampleWithLicensingValidator::Reverse(const wchar_t* input)
{
  ON_wString str;

  if (nullptr == input || 0 == input[0])
    return str;

  str = input;
  str.MakeReverse();

  return str;
}

//
// END SampleWithLicensingValidator license validator
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
