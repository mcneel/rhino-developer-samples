# Rhino3dm on Azure Functions
This is a very minimal project to demonstrate how to use Rhino3dm in Azure Functions.

Notes: 
- This example is based on this tutorial: https://learn.microsoft.com/en-us/azure/azure-functions/create-first-function-vs-code-csharp

## Useage
This assumes you have the appropriate Azure account and permissions, and understand how to authenticate Azure. Also assumes you are using VS Code and have installed all of the requirements listed in the above noted tutorial.

1. In the Azure panel of VS Code, create a new function app as noted in the tutorial.
2. Deploy the local project to a Function App.
3. When executing the function, you will get asked to provide a request body in the form of `{"name":"Azure"}`. Change this to `{"Radius":"<some number>"}`. For example: `{"Radius":"123.45"}`.
4. VS Code should produce a popup with the result of the function.