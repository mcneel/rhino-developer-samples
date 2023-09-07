# Rhino3dm on AWS Lambda
This is a very minimal project to demonstrate how to use Rhino3dm in AWS Lambda.

Notes: 
- At the time of writing, 8.0.0-beta2 on nuget includes a faulty librhino3dm_native.so file. This example references manually copied results from our GitHub Actions Release workflow, "rhino3dm.net nupkg" (librhino3dm.so from runtimes/linux-x64/native and Rhino.dll from lib/netstandard2.0).
- At the time of writing, we only support the linux-x64 runtime (not linux-arm64).

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