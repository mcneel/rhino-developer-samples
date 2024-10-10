using Rhino;
using Rhino.Collections;
using Rhino.Commands;

namespace SampleCsUserData.Commands
{
  public class SampleCsDocumentDictionary : Command
  {
    private const string INTEGER_VALUE = "INT";
    private const string DOUBLE_VALUE = "DBL";

    private int m_integer_value = 1;
    private double m_double_value = 2.13;

    public override string EnglishName
    {
      get { return "SampleCsDocumentDictionary"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetValues(SampleCsUserDataPlugIn.Instance.Dictionary);
      SetValues(SampleCsUserDataPlugIn.Instance.Dictionary);
      return Result.Success;
    }

    private void GetValues(ArchivableDictionary dictionary)
    {
      if (null == dictionary)
        return;

      RhinoApp.WriteLine(dictionary.Name);

      int integer_value;
      if (dictionary.TryGetInteger(INTEGER_VALUE, out integer_value))
      {
        m_integer_value = integer_value;
        RhinoApp.WriteLine("  Integer = {0}", m_integer_value);
      }
      else
        RhinoApp.WriteLine("  Integer = <none>");

      double double_value;
      if (dictionary.TryGetDouble(DOUBLE_VALUE, out double_value))
      {
        m_double_value = double_value;
        RhinoApp.WriteLine("  Double = {0}", m_double_value);
      }
      else
        RhinoApp.WriteLine("  Double = <none>");
    }

    private void SetValues(ArchivableDictionary dictionary)
    {
      if (null == dictionary)
        return;

      m_integer_value++;
      m_double_value *= 1.57;

      dictionary.Set(INTEGER_VALUE, m_integer_value);
      dictionary.Set(DOUBLE_VALUE, m_double_value);
    }
  }
}