using Rhino;

namespace examples_cs
{
  public class ex_selbyusertext : Rhino.Commands.SelCommand
  {
    public override string EnglishName{ get { return "SelByUserText"; } }

    string m_key = string.Empty;
    protected override Rhino.Commands.Result RunCommand(RhinoDoc doc, Rhino.Commands.RunMode mode)
    {
      // You don't have to override RunCommand if you don't need any user input. In
      // this case we want to get a key from the user. If you return something other
      // than Success, the selection is canceled
      return Rhino.Input.RhinoGet.GetString("key", true, ref m_key);
    }

    protected override bool SelFilter(Rhino.DocObjects.RhinoObject rhObj)
    {
      string s = rhObj.Attributes.GetUserString(m_key);
      return !string.IsNullOrEmpty(s);
    }
  }
}

