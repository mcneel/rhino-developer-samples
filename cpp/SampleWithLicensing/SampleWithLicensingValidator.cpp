#include "stdafx.h"
#include "SampleWithLicensingPlugIn.h"
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

  // Returns the build type of this plug-in
  CRhinoLicenseValidator::product_build_type ProductBuildType() override;
  
  // Called by Rhino when it needs your plug-in to validate your product key or license.
  CRhinoLicenseValidator::result VerifyLicenseKey(
    const wchar_t* licenseKey, 
    const wchar_t* validationCode, 
    const double validationCodeInstalledDate, 
    bool gracePeriodExpired
  ) override;
  
  // Called by Rhino after a call to VerifyLicenseKey that sets 
  // CRhinoLicenseValidator::m_requires_previous_version_license_verification to true.
  bool VerifyPreviousVersionLicense(
    const wchar_t* licenseKey,
    const wchar_t* previousVersionLicenseKey
  ) override;

  // Called by Rhino when Rhino Accounts gets a new lease, this function is called.
  void OnLeaseChanged(CRhinoLeaseChangedEventArgs&) override;

  // This member is obsolete.
  CRhinoLicenseValidator::result ValidateProductKey(const wchar_t* product_key) override;

private:
  bool ValidateSampleLicenseKey(const wchar_t* licenseKey);
};

// The one and only CSampleWithLicensingValidator object
static class CSampleWithLicensingValidator theSampleWithLicensingValidator;

// Constructor
CSampleWithLicensingValidator::CSampleWithLicensingValidator()
{
  m_date_to_expire = 0.0;
  m_license_count = 1;
  // This sample does not support Rhino accounts.
  m_requires_online_validation = false;
  // This sample does not require a previous license.
  m_requires_previous_version_license_verification = false;
}

// Returns the build type of this plug-in
CRhinoLicenseValidator::product_build_type CSampleWithLicensingValidator::ProductBuildType()
{
  return CRhinoLicenseValidator::release_build;
}

// This member is called by Rhino, from CRhinoPlugIn::GetLicense(), when it
// needs your plug-in to validate your product key or license.
CRhinoLicenseValidator::result CSampleWithLicensingValidator::VerifyLicenseKey(
  const wchar_t* licenseKey, 
  const wchar_t* validationCode, 
  const double validationCodeInstalledDate, 
  bool gracePeriodExpired
)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  // This icon will displayed in the "Licenses" page in the Options dialog.
  // TODO: provide a a product icon here.
  if (0 == m_product_icon)
  {
    int size = CRhinoDpi::IconSize(CRhinoDpi::IconType::NormalIcon);
    m_product_icon = CRhinoDpi::LoadIcon(AfxGetInstanceHandle(), IDI_MAIN, size);
  }

  CString license_key(licenseKey);
  CString validation_code(validationCode);

  bool bEvaluation = false;

  // Verify license key
  if (license_key.IsEmpty())
  {
    license_key = L"SAMPLE-EVALUATION";
    bEvaluation = true;
  }
  else if (license_key.Left(8).CompareNoCase(L"EVALUATION") == 0)
  {
    bEvaluation = true;
  }
  else if (!ValidateSampleLicenseKey(licenseKey))
  {
    m_error_message = L"The license key is invalid.";
    return CRhinoLicenseValidator::error_show_message;
  }

  // Verify validation code (if provided)
  if (!validation_code.IsEmpty())
  {
    if (validation_code.CompareNoCase(L"invalid") == 0)
    {
      m_error_message = L"Your validation code is invalid.";
      return CRhinoLicenseValidator::error_show_message;
    }
  }

  // Fill in the class members based on the license key
  // and validation code.

  // This value will never be display in any user interface.
  // When your plugin's VerifyLicenseKey function is called, it is
  // passed a a product, or CD, key that was entered into the Zoo
  // administrator console. Your VerifyLicenseKey function will validate
  // the product key and decode it into a product license. This is
  // where you can store this license. This value will be passed
  // to your application at runtime when it requests a license.
  m_product_license = license_key;

  // This value will display in user interface items, such as in
  // the Zoo console and in About dialog boxes. Also, this value
  // is used to uniquely identify this license. Thus, it is
  // critical that this value be unique per product key, entered
  // by the administrator. No other license of this product, as
  // validated by this plugin, should return this value.
  // TODO: provide an obfuscated license string here.
  m_serial_number = license_key;

  // This value will display in user interface items, such as in
  // the Zoo console and in About dialog boxes.
  // (e.g. "Rhinoceros 6", "Rhinoceros 6 Commercial", etc.)
  // TODO: provide a license title string here.
  m_license_title = SampleWithLicensingPlugIn().PlugInName();

  // The Zoo supports licenses that expire. If your licensing scheme
  // is sophisticated enough to support this, then specify the
  // expiration date here. Note, this value must be specified in
  // Coordinated Universal Time (UTC). If your license does not expire,
  // then just this value to null.
  if (bEvaluation)
  {
    COleDateTime expire_date = COleDateTime::GetCurrentTime();
    COleDateTimeSpan interval;
    interval.SetDateTimeSpan(90, 0, 0, 0);
    m_date_to_expire = expire_date + interval;
  }

  return CRhinoLicenseValidator::success;
}

// Called by Rhino, from CRhinoPlugIn::GetLicense(), after a call to VerifyLicenseKey that
// sets m_requires_previous_version_license_verification to true.
bool CSampleWithLicensingValidator::VerifyPreviousVersionLicense(
  const wchar_t* licenseKey,
  const wchar_t* previousVersionLicenseKey
)
{
  CString license_key(licenseKey);
  CString previous_version_license(previousVersionLicenseKey);

  if (license_key.IsEmpty())
    return false;
  if (previous_version_license.IsEmpty())
    return false;

  // For this example, we'll consider any non-empty previous version license to be valid.
  return true;
}

// When Rhino Accounts gets a new lease, this function is called.
void CSampleWithLicensingValidator::OnLeaseChanged(CRhinoLeaseChangedEventArgs& args)
{
  // This sample current does not support Rhino accounts.

  CRhinoLicenseLease* pLease = args.GetLease();
  if (nullptr == pLease)
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

// This member is obsolete.
CRhinoLicenseValidator::result CSampleWithLicensingValidator::ValidateProductKey(const wchar_t* product_key)
{
  return CRhinoLicenseValidator::result::error_show_message;
}

// Our license key validation code. In this example, we'll accept
// anything but a null or empty string.
bool CSampleWithLicensingValidator::ValidateSampleLicenseKey(const wchar_t* licenseKey)
{
  if (nullptr == licenseKey || 0 == licenseKey)
    return false;
  return true;
}

//
// END SampleWithLicensingValidator license validator
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
