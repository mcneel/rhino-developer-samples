Imports System
Imports System.Reflection
Imports System.Runtime.InteropServices
Imports Rhino.PlugIns

' Plug-In title and Guid are extracted from the following two attributes
<Assembly: AssemblyTitle("examples_vb")> 
<Assembly: Guid("da01c4b6-d776-4c8a-ab68-d372586dc54a")> 

' Plug-in Description Attributes - all of these are optional
<Assembly: PlugInDescription(DescriptionType.Address, "3670 Woodland Park Ave N" & vbCrLf & "Seattle" & vbCrLf & "WA" & vbCrLf & "98103")> 
<Assembly: PlugInDescription(DescriptionType.Country, "USA")> 
<Assembly: PlugInDescription(DescriptionType.Email, "steve@mcneel.com")> 
<Assembly: PlugInDescription(DescriptionType.Phone, "206.545.7000")> 
<Assembly: PlugInDescription(DescriptionType.Fax, "206.545.7321")> 
<Assembly: PlugInDescription(DescriptionType.Organization, "Robert McNeel & Associates")> 
<Assembly: PlugInDescription(DescriptionType.UpdateUrl, "http://www.rhino3d.com")> 
<Assembly: PlugInDescription(DescriptionType.WebSite, "http://www.rhino3d.com")> 

' General Information about an assembly is controlled through the following 
' set of attributes. Change these attribute values to modify the information
' associated with an assembly.

' Review the values of the assembly attributes

<Assembly: AssemblyDescription("examples_vb")> 
<Assembly: AssemblyCompany("Robert McNeel & Associates")> 
<Assembly: AssemblyProduct("examples_vb")> 
<Assembly: AssemblyCopyright("Copyright ©  2011")> 
<Assembly: AssemblyTrademark("")> 

<Assembly: ComVisible(False)>

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

<Assembly: AssemblyVersion("1.0.*")> 
<Assembly: AssemblyFileVersion("1.0.0.0")> 
