# Rhino3dm on AWS Lambda
This is a very minimal project to demonstrate how to use Rhino3dm in AWS Lambda.

Notes: 
- This example references the Rhino3dm package from NuGet, which includes librhino3dm_native.so for linux-x64 and linux-arm64 (runtimes/linux-x64/native, runtimes/linux-arm64/native).
- The function is deployed for x86_64 (`function-architecture` in aws-lambda-tools-defaults.json). Set it to `arm64` to run on Graviton.

## Useage
This assumes you have the appropriate AWS account and permissions, and understand how to authenticate the AWS CLI.

1. Deploy function:
`dotnet lambda deploy-function rhino3dm_aws_test`
2. Invoke function:
`dotnet lambda invoke-function rhino3dm_aws_test --payload "1234"`
    
    Which results in something like:
    ````
    Payload:
    "7753.450669059609"
    ````