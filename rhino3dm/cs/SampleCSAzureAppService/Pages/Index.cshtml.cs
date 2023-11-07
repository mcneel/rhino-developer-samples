using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Rhino;

namespace SampleCSAzureAppService.Pages;

public class IndexModel : PageModel
{
    private readonly ILogger<IndexModel> _logger;

    [ViewData]
    public string CircumferenceString { get; set; } = "0.00";

    public IndexModel(ILogger<IndexModel> logger)
    {
        _logger = logger;
        _logger.LogInformation("Let's Gooo!");
        CircumferenceString = "?";
    }

    public void OnGet()
    {

    }

    public IActionResult OnPost(Data data)
    {
        _logger.LogInformation("Received Something");
        _logger.LogInformation(data.Slider.ToString());
        data.Circumference = doSomethingWithRhino(data.Slider);
        return new JsonResult(data);
    }


    public double doSomethingWithRhino(double radius = 50) 
    {
        var circle = new Rhino.Geometry.Circle(radius);
        _logger.LogInformation("Circumference is: {0}", circle.Circumference);
        return circle.Circumference;
    }

    public class Data
    {
        public double Slider { get; set; }
        public double Circumference { get; set; }
    }

}
