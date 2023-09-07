using Amazon.Lambda.Core;

// Assembly attribute to enable the Lambda function's JSON input to be converted into a .NET class.
[assembly: LambdaSerializer(typeof(Amazon.Lambda.Serialization.SystemTextJson.DefaultLambdaJsonSerializer))]

namespace rhino3dm_aws_test;

public class Function
{

    /// <summary>
    /// A function that creates a circle and returns its circumference
    /// </summary>
    /// <param name="input">A value for the radius as a string</param>
    /// <returns>The circle circumference as a string</returns>
    public string FunctionHandler(string input)
    {
        double radius;
        if( Double.TryParse(input, out radius) ) {
            var circle = new Rhino.Geometry.Circle(radius);
            return circle.Circumference.ToString();
        } else {
            return "Could not parse input";
        }
    }
}
