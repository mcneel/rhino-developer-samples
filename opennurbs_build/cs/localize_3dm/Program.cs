using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace localize_3dm
{
  class Program
  {
    static int Main(string[] args)
    {
      if (args.Length < 1)
      {
        var exe_name = System.AppDomain.CurrentDomain.FriendlyName;
        Console.WriteLine("Syntax: {0} -action:[extract|localize] -xml:XML -3dm:MODEL [-targetDir:Output]", exe_name);
        return 1;
      }

      string action = "", model = "", xml = "", targetDir = "";

      foreach (string arg in args)
      {
        if (arg.StartsWith("-action:", StringComparison.InvariantCultureIgnoreCase))
          action = arg.Substring("-action:".Length).Trim();
        else if (arg.StartsWith("-3dm:", StringComparison.InvariantCultureIgnoreCase))
          model = arg.Substring("-3dm:".Length).Trim();
        else if (arg.StartsWith("-xml:", StringComparison.InvariantCultureIgnoreCase))
          xml = arg.Substring("-xml:".Length).Trim();
        else if (arg.StartsWith("-targetDir:", StringComparison.InvariantCultureIgnoreCase))
          targetDir = arg.Substring("-targetDir:".Length).Trim();
      }

      switch (action)
      {
        case "extract":
          return Extract(xml, model);
        case "localize":
          return Localize(xml, model, targetDir);
        default:
          Console.Error.WriteLine("Unrecognized action");
          return 1;
      }
    }

    static int Localize(string xml, string sourceModel, string targetDir)
    {
      XmlDocument doc = new XmlDocument();
      doc.Load(xml);
      var docElem = doc.DocumentElement;

      using (var file3dm = Rhino.FileIO.File3dm.Read(sourceModel))
      {
        if (file3dm == null)
        {
          Console.Error.WriteLine("Unable to open file '{0}'", sourceModel);
          return 1;
        }
        foreach (var layer in file3dm.Layers)
        {
          var path = string.Format("//RhinoModel/Layers/Layer[@English='{0}']", layer.Name);
          var nameNode = (XmlElement)doc.SelectSingleNode(path);
          if (nameNode != null)
            layer.Name = nameNode.GetAttribute("Localized");
        }
        foreach (var dimstyle in file3dm.DimStyles)
        {
          var path = string.Format("//RhinoModel/DimStyles/DimStyle[@English='{0}']", dimstyle.Name);
          var nameNode = (XmlElement)doc.SelectSingleNode(path);
          if (nameNode != null)
            dimstyle.Name = nameNode.GetAttribute("Localized");
        }
        foreach (var linetype in file3dm.Linetypes)
        {
          var path = string.Format("//RhinoModel/Linetypes/Linetype[@English='{0}']", linetype.Name);
          var nameNode = (XmlElement)doc.SelectSingleNode(path);
          if (nameNode != null)
            linetype.Name = nameNode.GetAttribute("Localized");
        }
        foreach (var hatchpattern in file3dm.HatchPatterns)
        {
          var path = string.Format("//RhinoModel/HatchPatterns/HatchPattern[@English='{0}']", hatchpattern.Name);
          var nameNode = (XmlElement)doc.SelectSingleNode(path);
          if (nameNode != null)
            hatchpattern.Name = nameNode.GetAttribute("Localized");
        }
        foreach (var material in file3dm.Materials)
        {
          var path = string.Format("//RhinoModel/Materials/Material[@English='{0}']", material.Name);
          var nameNode = (XmlElement)doc.SelectSingleNode(path);
          if (nameNode != null)
            material.Name = nameNode.GetAttribute("Localized");
        }

        // TODO: Handle These:
        /*
         * Texture Names
         * Environment Names
         */

        foreach (var view in file3dm.Views)
        {
          var path = string.Format("//RhinoModel/Viewports/Viewport[@English='{0}']", view.Name);
          var nameNode = (XmlElement)doc.SelectSingleNode(path);
          if (nameNode != null)
            view.Name = nameNode.GetAttribute("Localized");
        }

        foreach (var obj in file3dm.Objects)
        {
          var path = string.Format("//RhinoModel/Objects/Object[@English='{0}']", obj.Attributes.Name.Replace("'", "&apos;"));
          var node = (XmlElement)doc.SelectSingleNode(path);
          if (node != null)
            obj.Attributes.Name = node.GetAttribute("Localized");
        }

        var notesNode = (XmlElement)doc.SelectSingleNode("//RhinoModel/Notes");
        if (notesNode != null)
        {
          file3dm.Notes.Notes = notesNode.InnerText;
        }

        string filename = docElem.GetAttribute("Filename");
        if (string.IsNullOrEmpty(filename))
          filename = System.IO.Path.GetFileName(sourceModel);

        file3dm.Polish();
        file3dm.Write(System.IO.Path.Combine(targetDir, filename), 5);

        return 0;
      }
    }

    static int Extract(string xml, string model)
    {
      XmlDocument doc = new XmlDocument();
      doc.AppendChild(doc.CreateXmlDeclaration("1.0", "utf-8", "yes"));
      XmlElement body = (XmlElement)doc.AppendChild(doc.CreateElement("RhinoModel"));
      XmlElement filename = (XmlElement)body.AppendChild(doc.CreateElement("Filename"));
      filename.SetAttribute("English", System.IO.Path.GetFileName(model));
      filename.SetAttribute("Localized", System.IO.Path.GetFileName(model));

      if (!System.IO.File.Exists(model))
      {
        Console.WriteLine("Error: File does not exist: " + model);
        return 1;
      }

      using (var file3dm = Rhino.FileIO.File3dm.Read(model))
      {
        if (file3dm == null)
        {
          Console.Error.WriteLine("Unable to open file '{0}'", model);
          return 1;
        }

        XmlElement layers = (XmlElement)body.AppendChild(doc.CreateElement("Layers"));
        foreach (var layer in file3dm.Layers)
        {
          XmlElement layerElem = (XmlElement)layers.AppendChild(doc.CreateElement("Layer"));
          layerElem.SetAttribute("English", layer.Name);
          layerElem.SetAttribute("Localized", layer.Name);
        }

        XmlElement dimstyles = (XmlElement)body.AppendChild(doc.CreateElement("DimStyles"));
        foreach (var dimstyle in file3dm.DimStyles)
        {
          XmlElement dimstyleElem = (XmlElement)dimstyles.AppendChild(doc.CreateElement("DimStyle"));
          dimstyleElem.SetAttribute("English", dimstyle.Name);
          dimstyleElem.SetAttribute("Localized", dimstyle.Name);
        }

        XmlElement linetypes = (XmlElement)body.AppendChild(doc.CreateElement("Linetypes"));
        foreach (var linetype in file3dm.Linetypes)
        {
          XmlElement linetypeElem = (XmlElement)linetypes.AppendChild(doc.CreateElement("Linetype"));
          linetypeElem.SetAttribute("English", linetype.Name);
          linetypeElem.SetAttribute("Localized", linetype.Name);
        }

        XmlElement hatchpatterns = (XmlElement)body.AppendChild(doc.CreateElement("HatchPatterns"));
        foreach (var hatchpattern in file3dm.HatchPatterns)
        {
          XmlElement hatchpatternElem = (XmlElement)hatchpatterns.AppendChild(doc.CreateElement("HatchPattern"));
          hatchpatternElem.SetAttribute("English", hatchpattern.Name);
          hatchpatternElem.SetAttribute("Localized", hatchpattern.Name);
        }

        XmlElement materials = (XmlElement)body.AppendChild(doc.CreateElement("Materials"));
        foreach (var material in file3dm.Materials)
        {
          XmlElement materialElem = (XmlElement)materials.AppendChild(doc.CreateElement("Material"));
          materialElem.SetAttribute("English", material.Name);
          materialElem.SetAttribute("Localized", material.Name);
        }

        XmlNode views = (XmlElement)body.AppendChild(doc.CreateElement("Viewports"));
        foreach (var view in file3dm.Views)
        {
          XmlElement viewNode = (XmlElement)views.AppendChild(doc.CreateElement("Viewport"));
          viewNode.SetAttribute("English", view.Name);
          viewNode.SetAttribute("Localized", view.Name);
        }

        XmlNode objects = (XmlElement)body.AppendChild(doc.CreateElement("Objects"));
        foreach (var obj in file3dm.Objects)
        {
          if (string.IsNullOrEmpty(obj.Attributes.Name))
            continue;
          XmlElement objNode = (XmlElement)objects.AppendChild(doc.CreateElement("Object"));
          objNode.SetAttribute("ID", obj.Attributes.ObjectId.ToString());
          objNode.SetAttribute("English", obj.Attributes.Name);
          objNode.SetAttribute("Localized", obj.Attributes.Name);
        }
        //if (null != file3dm.NamedViews)
        //{
        //  XmlNode namedViews = (XmlElement)body.AppendChild(doc.CreateElement("NamedViews"));
        //  foreach (var view in file3dm.NamedViews)
        //  {
        //    XmlElement viewNode = (XmlElement)namedViews.AppendChild(doc.CreateElement("NamedView"));
        //    viewNode.SetAttribute("English", view.Name);
        //    viewNode.SetAttribute("Localized", view.Name);
        //  }
        //}

        XmlNode notes = (XmlElement)body.AppendChild(doc.CreateElement("Notes"));
        notes.InnerText = file3dm.Notes.Notes;

        doc.Save(xml);
        return 0;
      }      
    }
  }

}