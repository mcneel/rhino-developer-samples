using Xunit;
using Amazon.Lambda.Core;
using Amazon.Lambda.TestUtilities;

namespace rhino3dm_aws_test.Tests;

public class FunctionTest
{
    [Fact]
    public void TestCircleFunction()
    {

        // Invoke the lambda function and confirm the string was upper cased.
        var function = new Function();
        var result = function.FunctionHandler("1234");

        Assert.Equal("7753.450669059609", result);
    }
}
