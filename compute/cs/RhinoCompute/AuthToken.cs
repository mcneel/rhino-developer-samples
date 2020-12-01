using System;
namespace Rhino.Compute
{
    public static class AuthToken
    {
        public static string Get ()
        {
            // Set RHINO_COMPUTE_TOKEN environment variable or provide your token here

            String tokenFromEnv = System.Environment.GetEnvironmentVariable("RHINO_COMPUTE_TOKEN");
            if (!String.IsNullOrEmpty (tokenFromEnv))
            {
                return tokenFromEnv;
            } else
            {
                return "<Add authorization token here if you don't have the environment varible>";
            }

        }
    }
}
