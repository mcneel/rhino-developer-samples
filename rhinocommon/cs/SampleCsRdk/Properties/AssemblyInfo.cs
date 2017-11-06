using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Rhino.PlugIns;

// Plug-In title and Guid are extracted from the following two attributes
[assembly: AssemblyTitle("C# Test Render Plug-In (RDK)")]
[assembly: Guid("6886aaa4-88df-4957-af2c-f186eb0ee5d1")]

// Plug-in Description Attributes - all of these are optional
[assembly: PlugInDescription(DescriptionType.Address, "123 Programmer Lane\nCity\nState\n12345-6789")]
[assembly: PlugInDescription(DescriptionType.Country, "USA")]
[assembly: PlugInDescription(DescriptionType.Email, "product_support@mycompany.com")]
[assembly: PlugInDescription(DescriptionType.Phone, "123.456.7890")]
[assembly: PlugInDescription(DescriptionType.Fax, "987.654.3210")]
[assembly: PlugInDescription(DescriptionType.Organization, "Company Name")]
[assembly: PlugInDescription(DescriptionType.UpdateUrl, "http://updates.mycompany.com")]
[assembly: PlugInDescription(DescriptionType.WebSite, "http://www.mycompany.com")]

// General Information about an assembly is controlled through the following 
// set of attributes. Change these attribute values to modify the information
// associated with an assembly.
[assembly: AssemblyDescription("rdktest_cs")]
[assembly: AssemblyConfiguration("")]
[assembly: AssemblyCompany("Company Name")]
[assembly: AssemblyProduct("rdktest_cs")]
[assembly: AssemblyCopyright("Copyright ©  2010")]
[assembly: AssemblyTrademark("")]
[assembly: AssemblyCulture("")]

// Setting ComVisible to false makes the types in this assembly not visible 
// to COM components.  If you need to access a type in this assembly from 
// COM, set the ComVisible attribute to true on that type.
[assembly: ComVisible(false)]

// Version information for an assembly consists of the following four values:
//
//      Major Version
//      Minor Version 
//      Build Number
//      Revision
//
// You can specify all the values or you can default the Revision and Build Numbers 
// by using the '*' as shown below:
[assembly: AssemblyVersion("1.0.*")]
[assembly: AssemblyFileVersion("1.0.0.0")]
