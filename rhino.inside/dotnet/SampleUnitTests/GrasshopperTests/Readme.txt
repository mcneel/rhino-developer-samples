Notes:

(1) The Visual Studio XUnit template wizard only includes options for .Net Core projects. After creating your XUnit test project, you must
	right-click on the project in SOlution Explorer and select 'Edit Project File'. You will see a couple of lines near the top that look
	like this:

		<PropertyGroup>
			<TargetFramework>net5.0</TargetFramework>

	Change "net5.0" to "net48" for Rhino7 tests.

(2) You need to set the project platform to "x64"

(3) You must include a file called "xunit.runner.json" to configure XUnit to NOT use AppDomains. 
If your tests depend from UI, Rhino commands, or any shared state, you should also set XUnit not to parallelize the tests.
You can achieve these two by adding a text file to the project with that name, that looks like this:

		{
			"$schema": "https://xunit.net/schema/current/xunit.runner.schema.json",
			"appDomain": "denied"
			"parallelizeAssembly": false,
			"parallelizeTestCollections": false
		}

	Additional xunit options may be added (see https://xunit.net/docs/configuration-files). You must also ensure that this file is copied
	to your project output directory. Select xunit.runner.json in solution explorer and select 'Copy if newer'
 
(4) You must add Rhino.Inside to your test project using NuGet

(5) This example also uses the same mechanism as in (3) to copy relevant grassopper files into the output directory, but you can use a hard-coded
	path if you chose.

