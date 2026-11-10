# rhino3dm .NET samples

Each folder is a small program that uses the [Rhino3dm](https://www.nuget.org/packages/Rhino3dm) NuGet package. They need the .NET 10 SDK.

Open `Rhino3dmSamples.sln` to see every sample, or build and run one from its folder:

```
cd SampleCSReadFile
dotnet run
```

Two files in this folder apply to every sample:

- `Directory.Packages.props` sets the package versions, so a project's `PackageReference` has no `Version`. To update Rhino3dm, change its version there.
- `Directory.Build.props` sets the target framework (`net10.0`), implicit usings and nullable reference types.

To use a sample on its own, copy its folder together with these two files, or add the `Version` to each `PackageReference` and the properties to the project file.

| Sample | What it shows |
| --- | --- |
| [SampleCSAddInstanceDefinition](SampleCSAddInstanceDefinition) | Add a block (instance definition) and a block instance |
| [SampleCSAWSLambda](SampleCSAWSLambda) | Use Rhino3dm in an AWS Lambda function |
| [SampleCSAzureAppService](SampleCSAzureAppService) | Use Rhino3dm in an Azure App Service web app |
| [SampleCSAzureFunction](SampleCSAzureFunction) | Use Rhino3dm in an Azure Function |
| [SampleCSEmbeddedFiles](SampleCSEmbeddedFiles) | Read and save the files embedded in a model |
| [SampleCSExportObj](SampleCSExportObj) | Export the meshes in a model to OBJ |
| [SampleCSJoinCurves](SampleCSJoinCurves) | Join the curves in a model |
| [SampleCSPBRMaterial](SampleCSPBRMaterial) | Add a physically based material |
| [SampleCSReadFile](SampleCSReadFile) | Read a 3dm file |
| [SampleCSReadTextEntities](SampleCSReadTextEntities) | Read the text entities in a model |
| [SampleCSTestSetMesh](SampleCSTestSetMesh) | Set the render meshes on breps and extrusions |
| [SampleCSViewports](SampleCSViewports) | Read a model's views and named views |
