using System;
using System.Drawing;
using Grasshopper.Kernel;

namespace SampleGhTaskCapable
{
  public class SampleGhTaskCapableInfo : GH_AssemblyInfo
  {
    public override string Name => "SampleGhTaskCapable";

    public override Bitmap Icon
    {
      get
      {
        //Return a 24x24 pixel bitmap to represent this GHA library.
        return null;
      }
    }
    public override string Description
    {
      get
      {
        //Return a short string describing the purpose of this GHA library.
        return "";
      }
    }
    public override Guid Id
    {
      get
      {
        return new Guid("e0332029-ae49-484c-8ceb-be9b29636df1");
      }
    }

    public override string AuthorName
    {
      get
      {
        //Return a string identifying you or your company.
        return "";
      }
    }
    public override string AuthorContact
    {
      get
      {
        //Return a string representing your preferred contact details.
        return "";
      }
    }
  }
}
