using System.Net;
using Microsoft.Azure.Functions.Worker;
using Microsoft.Azure.Functions.Worker.Http;
using Microsoft.Extensions.Logging;
using Newtonsoft.Json;
using System.Text.Json.Serialization;
using System.Threading.Tasks;
using System.Dynamic;

namespace McNeel.Function1
{
    public class HttpTrigger
    {
        private readonly ILogger _logger;

        public HttpTrigger(ILoggerFactory loggerFactory)
        {
            _logger = loggerFactory.CreateLogger<HttpTrigger>();
        }

        [Function("HttpTrigger")]
        public async Task<HttpResponseData> Run([HttpTrigger(AuthorizationLevel.Anonymous, "get", "post")] HttpRequestData req, FunctionContext context)
        {
            _logger.LogInformation("C# HTTP trigger function processed a request.");

            string body = await new StreamReader(req.Body).ReadToEndAsync();
            var json = JsonConvert.DeserializeObject<Data>(body);

            _logger.LogInformation(json.Radius);

            var response = req.CreateResponse(HttpStatusCode.OK);
            response.Headers.Add("Content-Type", "text/plain; charset=utf-8");

            double radius;
            if( Double.TryParse(json.Radius, out radius) ) {
                var circle = new Rhino.Geometry.Circle(radius);
                response.WriteString(circle.Circumference.ToString());
            } else {
                response.WriteString("Could not parse input");
            }

            return response;
        }
    }

    public class Data
    {
        public string? Radius { get; set; }

    }
}
