using Rhino;
using Rhino.Commands;

namespace examples_cs
{
  public class DetermineCurrentLanguageCommand : Command
  {
    public override string EnglishName { get { return "csCurrentLanguage"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var language_id = Rhino.ApplicationSettings.AppearanceSettings.LanguageIdentifier;
      var culture = new System.Globalization.CultureInfo(language_id);
      RhinoApp.WriteLine("The current language is {0}", culture.EnglishName);
      return Result.Success;
    }
  }
}