Imports System
Imports System.Reflection
Imports System.Runtime.InteropServices
Imports Rhino.PlugIns

' Plug-in Description Attributes - all of these are optional.
' These will show in Rhino's option dialog, in the tab Plug-ins.
<Assembly: PlugInDescription(DescriptionType.Address, "-")>
<Assembly: PlugInDescription(DescriptionType.Country, "-")>
<Assembly: PlugInDescription(DescriptionType.Email, "-")>
<Assembly: PlugInDescription(DescriptionType.Phone, "-")>
<Assembly: PlugInDescription(DescriptionType.Fax, "-")>
<Assembly: PlugInDescription(DescriptionType.Organization, "-")>
<Assembly: PlugInDescription(DescriptionType.UpdateUrl, "-")>
<Assembly: PlugInDescription(DescriptionType.WebSite, "-")>

' Icons should be Windows .ico files And contain 32-bit images in the following sizes 16, 24, 32, 48, And 256.
' This is a Rhino 6-only description.
<Assembly: PlugInDescription(DescriptionType.Icon, "SampleVbWinForms.EmbeddedResources.plugin-utility.ico")>

' General Information about an assembly is controlled through the following 
' set of attributes. Change these attribute values to modify the information
' associated with an assembly.
<Assembly: AssemblyTitle("SampleVbWinForms")>

' This will be used also for the plug-in description.
<Assembly: AssemblyDescription("SampleVbWinForms utility plug-in")>

<Assembly: AssemblyConfiguration("")>
<Assembly: AssemblyCompany("Robert McNeel & Associates")>
<Assembly: AssemblyProduct("SampleVbWinForms")>
<Assembly: AssemblyCopyright("Copyright ©  2018, Robert McNeel & Associates")>
<Assembly: AssemblyTrademark("")>
<Assembly: AssemblyCulture("")>

' Setting ComVisible to false makes the types in this assembly not visible 
' to COM components.  If you need to access a type in this assembly from 
' COM, set the ComVisible attribute to true on that type.
<Assembly: ComVisible(False)>

'The following GUID is for the ID of the typelib if this project is exposed to COM
<Assembly: Guid("28cdf2ae-3433-47d0-9014-f160059cf390")> ' This will also be the Guid of the Rhino plug-in

' Version information for an assembly consists of the following four values:
'
'      Major Version
'      Minor Version 
'      Build Number
'      Revision
'
' You can specify all the values or you can default the Build and Revision Numbers 
' by using the '*' as shown below:
' <Assembly: AssemblyVersion("1.0.*")> 

<Assembly: AssemblyVersion("1.0.0.0")>
<Assembly: AssemblyFileVersion("1.0.0.0")>

' Make compatible With Rhino Installer Engine
<Assembly: AssemblyInformationalVersion("2")>
