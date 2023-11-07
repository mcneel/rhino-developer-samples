# rhino3dm.net on Azure Functions
This is a very minimal project to demonstrate how to use rhino3dm.net in Azure App Services.

Notes: 
- This example is based on this tutorial: https://learn.microsoft.com/en-us/azure/app-service/quickstart-dotnetcore?tabs=net70&pivots=development-environment-cli

## Useage
This assumes you have the appropriate Azure account and permissions, and understand how to authenticate Azure. Also assumes you are using VS Code and have installed all of the requirements listed in the above noted tutorial, notably, the Azure CLI.

The difference between the template app and this one is in `Pages/Index.cshtml` and `Pages/Index.cshtml.cs`

The application presents an input slider which when changed, sends the value to the back-end, uses the value as a radius for a circle, and returns the circumference. Then the value of the circumference is updated in the front-end.

1. To start the app locally, use: `dotnet run --urls=http://localhost:5001/`
2. To deploy the app to an App Service, use: 
    - `az login`
    - `az webapp up --sku F1 --name rhino3dm-appservice-test --os-type linux`
3. After a few minutes, the cli will respond with the some information about your app and the url
