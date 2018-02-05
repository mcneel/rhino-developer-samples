using System;
using Rhino.PlugIns;
using Rhino.UI;

namespace SampleCsWithLicense
{
  ///<summary>
  /// <para>Every RhinoCommon .rhp assembly must have one and only one PlugIn-derived
  /// class. DO NOT create instances of this class yourself. It is the
  /// responsibility of Rhino to create an instance of this class.</para>
  /// <para>To complete plug-in information, please also see all PlugInDescription
  /// attributes in AssemblyInfo.cs (you might need to click "Project" ->
  /// "Show All Files" to see it in the "Solution Explorer" window).</para>
  ///</summary>
  public class SampleCsWithLicensePlugIn : PlugIn
  {
    /// <summary>
    /// Public constructor
    /// </summary>
    public SampleCsWithLicensePlugIn()
    {
      Instance = this;
    }

    /// <summary>
    /// Gets the only instance of the SampleCsWithLicensePlugIn plug-in.
    /// </summary>
    public static SampleCsWithLicensePlugIn Instance
    {
      get; private set;
    }

    // You can override methods here to change the plug-in behavior on
    // loading and shut down, add options pages to the Rhino _Option command
    // and maintain plug-in wide options in a document.

    /// <summary>
    /// OnLoad override
    /// </summary>
    protected override LoadReturnCode OnLoad(ref string errorMessage)
    {
      var rc = GetLicense(Capabilities, TextMask, OnValidateProductKey, OnLeaseChanged);
      if (!rc)
        return LoadReturnCode.ErrorNoDialog;

      return LoadReturnCode.Success;
    }

    #region Licensing Info

    /// <summary>
    /// Returns an icon to show in the License page in Options.
    /// </summary>
    private static System.Drawing.Icon ProductIcon
    {
      get
      {
        var size = RhinoWindows.Forms.Dpi.ScaleInt(32);
        return DrawingUtilities.LoadIconWithScaleDown("SampleCsWithLicense.Resources.SampleCs.ico", size);
      }
    }

    /// <summary>
    /// In the event that the user needs to be asked for a license, then you can
    /// provide a text mask, which helps the user to distinguish between proper
    /// and improper user input of your license code. Note, if you do not want
    /// to use a text mask, then pass in a null value for this parameter.
    /// </summary>
    private static string TextMask => ">S\\AMP\\LE-AAAAAAAAAA";

    /// <summary>
    /// Our hard-code evaluation license key.
    /// </summary>
    private static string EvalLicenseKey => "SAMPLE-EVALUATION";

    /// <summary>
    /// Controls the buttons that will appear on the license notification window that
    //  is displayed if a license for the requesting product is not found. Note, the
    //  "Close" button will always be displayed.
    /// </summary>
    private static LicenseCapabilities Capabilities => LicenseCapabilities.CanBeEvaluated |
                                                       LicenseCapabilities.CanBePurchased |
                                                       LicenseCapabilities.CanBeSpecified;

    #endregion

    #region Licensing delegates

    /// <summary>
    /// Validates a product key or license.
    /// </summary>
    /// <param name="licenseKey">The license key as input by the user.</param>
    /// <param name="licenseData">The license data extracted by validating the license key.</param>
    /// <returns>The results of the license validation.</returns>
    private static ValidateResult OnValidateProductKey(string licenseKey, out LicenseData licenseData)
    {
      // Initialize output data
      licenseData = new LicenseData
      {
        BuildType = LicenseBuildType.Release,
        LicenseCount = 1,
        ProductIcon = ProductIcon,
        IsUpgradeFromPreviousVersion = false,
        RequiresOnlineValidation = false // This sample current does not support Rhino accounts.
      };

      var evaluation = false;
      if (string.IsNullOrEmpty(licenseKey))
      {
        licenseKey = EvalLicenseKey;
        evaluation = true;
      }
      else if (licenseKey.IndexOf("EVALUATION", StringComparison.OrdinalIgnoreCase) > 0)
      {
        evaluation = true;
      }

      // Fill in the class members based on the license key and validation code.
      // This value will never be display in any user interface.
      // When your plugin's VerifyLicenseKey function is called, it is
      // passed a a product, or CD, key that was entered into the Zoo
      // administrator console. Your VerifyLicenseKey function will validate
      // the product key and decode it into a product license. This is
      // where you can store this license. This value will be passed
      // to your application at runtime when it requests a license.
      licenseData.ProductLicense = licenseKey;

      // This value will display in user interface items, such as in
      // the Zoo console and in About dialog boxes. Also, this value
      // is used to uniquely identify this license. Thus, it is
      // critical that this value be unique per product key, entered
      // by the administrator. No other license of this product, as
      // validated by this plugin, should return this value.
      // TODO: provide an obfuscated license string here.
      licenseData.SerialNumber = licenseKey;

      // This value will display in user interface items, such as in
      // the Zoo console and in About dialog boxes.
      // (e.g. "Rhinoceros 6", "Rhinoceros 6 Commercial", etc.)
      // TODO: provide a license title string here.
      licenseData.LicenseTitle = Instance.Name;

      // The Zoo supports licenses that expire. If your licensing scheme
      // is sophisticated enough to support this, then specify the
      // expiration date here. Note, this value must be specified in
      // Coordinated Universal Time (UTC). If your license does not expire,
      // then just this value to null.
      if (evaluation)
      {
        var today = DateTime.UtcNow;
        var expire = today.AddDays(90);
        licenseData.DateToExpire = expire;
      }

      return ValidateResult.Success;
    }

    /// <summary>
    /// Called by Rhino to signal that a lease from Rhino Accounts has changed. 
    /// If LicenseLeaseChangedEventArgs.Lease is null, then the server has signaled
    /// that this product is no longer licensed. Your plug-in must change behavior 
    /// to behave appropriately.
    /// </summary>
    /// <param name="args">Data passed by Rhino when the lease changes</param>
    /// <param name="icon">Icon to be displayed in Tools > Options > Licenses for this lease.</param>
    private static void OnLeaseChanged(LicenseLeaseChangedEventArgs args, out System.Drawing.Icon icon)
    {
      icon = ProductIcon;

      // This sample does not support Rhino accounts.

      if (null == args.Lease)
      {
        // Lease has been voided; this product should behave as if it has no
        // license. It is up to the plug-in to determine what that looks like.
      }

      // Verify that args.Lease.ProductId is correct
      // Verify that args.Lease.ProductEdition is correct
      // Verify that args.Lease.ProductVersion is correct
      // Verify thatargs.Lease.IsExpired() is false
    }

    #endregion

  }
}