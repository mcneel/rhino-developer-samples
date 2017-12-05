Imports ZooPlugin

Public Class SampleVbZooClass : Implements IZooPlugin

  ''' <summary>
  ''' Returns a guid that uniquely identifies this Zoo plug-in class.
  ''' </summary>
  Public Function ZooPluginId() As System.Guid Implements IZooPlugin.ZooPluginId
    Return New Guid("95640f81-0a0d-44d3-9bd6-77a40061bbdb")
  End Function

  ''' <summary>
  ''' Returns the name of your company or organization.
  ''' (e.g. "Robert McNeel and Associates")
  ''' </summary>
  Public Function Organization() As String Implements IZooPlugin.Organization
    Return "Robert McNeel & Associates"
  End Function

  ''' <summary>
  ''' Returns the address of your company or organization.
  ''' (e.g. "3670 Woodland Park Avenue North, Seattle, WA 98115")
  ''' </summary>
  Public Function Address() As String Implements IZooPlugin.Address
    Return "3670 Woodland Park Avenue North" & vbCrLf & "Seattle, WA 98115"
  End Function

  ''' <summary>
  ''' Returns the country in which your company or organization is located.
  ''' (e.g. "United States")
  ''' </summary>
  Public Function Country() As String Implements IZooPlugin.Country
    Return "United States"
  End Function

  ''' <summary>
  ''' Returns an email address that a customer can use to contact you.
  ''' (e.g. "tech@mcneel.com")
  ''' </summary>
  Public Function Email() As String Implements IZooPlugin.Email
    Return "devsupport@mcneel.com"
  End Function

  ''' <summary>
  ''' Returns a phone number that a customer can use to contact you.
  ''' (e.g. "(206) 545-7000")
  ''' </summary>
  Public Function Phone() As String Implements IZooPlugin.Phone
    Return "(206) 545-6877"
  End Function

  ''' <summary>
  ''' Returns a FAX number that a customer can use to contact you. 
  ''' (e.g. "(206) 545-7321")
  ''' </summary>
  Public Function Fax() As String Implements IZooPlugin.Fax
    Return "(206) 545-7321"
  End Function

  ''' <summary>
  ''' Returns the web address or url of your company or organization.
  ''' (e.g. "http://www.rhino3d.com")
  ''' </summary>
  Public Function Web() As String Implements IZooPlugin.Web
    Return "http://www.rhino3d.com/zoo"
  End Function

  ''' <summary>
  ''' Returns your unique identifier, or PlugInId, of your Rhino plug-in.
  ''' If this does not return the same guid as your Rhino plug-in, then
  ''' it will not be able to obtain a license from the Zoo.
  ''' </summary>
  Public Function RhinoPluginId() As System.Guid Implements IZooPlugin.RhinoPluginId
    ' If your Rhino plug-in uses the C++ SDK, this is returned by your
    '    plug-in object's CRhinoPlugIn::PlugInID() override.
    '
    ' If your Rhino plug-in uses the .NET SDK, this is returned by your
    '    plug-in object's MRhinoPlugIn.PlugInID() override.
    '
    ' If your Rhino plug-in uses the RhinoCommon SDK, this is returned by your
    '    plug-in project's "Guid" assembly attribute.
    Return New Guid("402c2709-8aa6-4125-8787-e7dd869a3f74")
  End Function

  ''' <summary>
  ''' Returns the name, version, and/or type of the product that this plug-in
  ''' validates. This string will appear in user interfaces were one can 
  ''' choose type type of license to validate.
  ''' </summary>
  Public Function ProductTitle() As String Implements IZooPlugin.ProductTitle
    Return "SampleVbZoo 1.0"
  End Function

  ''' <summary>
  ''' Validates a product, or CD, key that was entered into the Zoo administrator
  ''' onsole, and returns license data. This data will, in turn, be serialized,
  ''' maintained, and distributed by the Zoo.
  ''' </summary>
  ''' <param name="productKey">
  ''' The product, or CD, key to validate. This is the raw, unmodified product key
  ''' string as entered into the Zoo Administrator console.
  ''' </param>
  ''' <param name="licenseData">
  ''' If productKey represents a valid license for your product, then licenseData
  ''' should be filled in with information about the license.
  ''' </param>
  ''' <returns>
  ''' Return 0 on success; the output Message is ignored.
  ''' Return any other number to indicate failure. 
  ''' The Zoo will call FormatErrorMessage with the value you return in order to get
  ''' a human-readable error message for display and logging purposes.
  ''' </returns>
  Public Function ValidateProductKey(productKey As String, ByRef licenseData As ZooPluginLicenseData) As Integer Implements IZooPlugin.ValidateProductKey

    ' This class contains information about your product's license.
    licenseData = New ZooPluginLicenseData()

    ' If this example, we won't do much valiation...
    If (String.IsNullOrEmpty(productKey)) Then
      Return -1
    End If

    ' This value will never be display in any user interface.
    ' When your plugin's ValidateProductKey member is called, it is
    ' passed a a product, or CD, key that was entered into the Zoo
    ' administrator console. Your ValidateProductKey will validate
    ' the product key and decode it into a product license. This is
    ' where you can store this license. This value will be passed
    ' to your application at runtime when it requests a license.
    licenseData.ProductLicense = productKey

    ' This value will display in user interface items, such as in
    ' the Zoo console and in About dialog boxes. Also, this value
    ' is used to uniquely identify this license. Thus, it is
    ' critical that this value be unique per product key, entered
    ' by the administrator. No other license of this product, as
    ' valided by this plugin, should return this value.
    '
    ' This example just scrambles the productKey...
    licenseData.SerialNumber = Scramble(productKey)

    ' This value will display in user interface items, such as in
    ' the Zoo console and in About dialog boxes.
    ' (e.g. "Rhinoceros 5.0", "Rhinoceros 5.0 Commercial", etc.)
    licenseData.LicenseTitle = "SampleVbZoo 1.0 Educational"

    ' The build of the product that this license work with.
    ' When your product requests a license from the Zoo, it
    ' will specify one of these build types.
    licenseData.BuildType = LicenseBuildType.Release

    ' Zoo licenses can be used by more than one instance of any application.
    ' For example, a single Rhion Education Lab license can be used by up
    ' to 30 systems simulaneously. If your license supports multiple instance,
    ' then specify the number of supported instances here. Otherwise just
    ' specify a value of 1 for single instance use.
    licenseData.LicenseCount = 1

    ' The Zoo supports licenses that expire. If your licensing scheme
    ' is sophisticated enough to support this, then specify the
    ' expiration date here. Note, this value must be speicified in
    ' Coordinated Universal Time (UTC). If your license does not expire,
    ' then just this value to null.
    licenseData.DateToExpire = Nothing

    Return 0

  End Function

  ''' <summary>
  ''' Looks up a human-readable, properly localized error message for logging and
  ''' display purposes.
  ''' </summary>
  ''' <param name="messageId">
  ''' MessageID is a plug-in specific ID that is returned when ValidateProductKey
  ''' fails to validate the key.
  ''' </param>
  ''' <param name="locale">
  ''' The CultureInfo associated with the currently running process; use this info
  ''' to localize your error messages appropriately.
  ''' </param>
  Public Function FormatErrorMessage(messageId As Integer, locale As System.Globalization.CultureInfo) As String Implements IZooPlugin.FormatErrorMessage
    Dim message As String = Nothing
    Select Case messageId
      Case -1
        message = "The license is invalid."
      Case Else
        message = "The license is invalid."
    End Select
    Return message
  End Function

  ''' <summary>
  ''' Randomizes character positions in string
  ''' </summary>
  Private Function Scramble(input As String) As String

    If String.IsNullOrEmpty(input) Then
      Return input
    End If

    Dim inputChars As New List(Of Char)(input)
    Dim outputChars As Char() = New Char(inputChars.Count - 1) {}

    Dim rand As New Random()

    For i As Integer = inputChars.Count - 1 To 0 Step -1
      Dim index As Integer = rand.[Next](i)
      outputChars(i) = inputChars(index)
      inputChars.RemoveAt(index)
    Next

    Return New String(outputChars)
  End Function
  
    ''' <summary>
    ''' We do not need to show any UI.
    ''' If we wanted to, we could show it here before the key is passed over
    ''' to <see cref="ValidateProductKey">ValidateProductKey</see>.
    ''' </summary>
    ''' <param name="productKey">The product, or CD, key to validate.
    ''' This is the raw, unmodified product key string as entered into the
    ''' Zoo Administrator console.</param>
    ''' <param name="validatedKey">The modified productKey string.
    ''' Again, in most cases you will set validatedKey equal productKey.
    ''' This value will be passed to ValidateProductKey().</param>
    ''' <returns>Return 0 on success; the output Message is ignored.
    ''' Return any other number to indicate failure. The Zoo will call
    ''' FormatErrorMessage with the value you return in order to get a
    ''' human-readable error message for display and logging purposes.
    ''' </returns>
  Public Function ValidateProductKeyUI(productKey As String, ByRef validatedKey As String) As Integer Implements IZooPlugin.ValidateProductKeyUI
    validatedKey = productKey
    Return 0
  End Function

End Class
