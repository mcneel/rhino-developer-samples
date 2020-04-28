using System;
using System.Linq;
using System.Text.RegularExpressions;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsDuplicateObjectFromNameTag : Command
  {
    public override string EnglishName => "SampleCsDuplicateObjectFromNameTag";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      //This command takes a Text annotation object which has an Object Name function associated with it (fx object name)
      //it then finds the object associated and creates a copy of it in the document with a new tag for the copy which
      //indexes the copy name as Object A > Object B > Object C.

      //Ask for a text object that's being used as a name tag
      var go = new GetObject() { GeometryFilter = ObjectType.Annotation };
      go.GeometryFilter = ObjectType.Annotation;
      go.SetCommandPrompt("Select Text with Object Name Function Applied");
      go.EnablePreSelect(false, true);
      go.GetMultiple(1, 0);

      if (go.Result() != GetResult.Object)
        return Result.Cancel;



      foreach (var ro in go.Objects().Select(ob => ob.Object()).ToList())
      {

        //Deselect our input object and filter out anything that isnt a text annotation
        doc.Objects.Select(ro.Id, false);
        if (ro.Geometry.ObjectType != ObjectType.Annotation) continue;
        if (!(ro.Geometry is TextEntity te)) continue;

        //See if the annotation contains the ingredients of a function
        var fx = te.RichText;
        if (!fx.Contains("%<") && !fx.Contains(">%")) continue;

        //Find the original doc object guid that tag function is linked to
        var object_id = ExtractGuidFromString(fx);
        var isguid = Guid.TryParse(object_id, out Guid obj_guid);
        if (!isguid) continue;

        //Create a duplicate of the linked text object
        var copy_obj = doc.Objects.Find(obj_guid);
        if (copy_obj == null) continue;

        //Duplicate the original objects geometry and attributes
        var duplicate = copy_obj.DuplicateGeometry();
        var attributes = copy_obj.Attributes.Duplicate();
        attributes.Name = $"{copy_obj.Name} {GetNextIndexName(obj_guid, ref doc)}";

        //Tag the copy with a reference to the original so that we can figure out the next letter index easier
        attributes.SetUserString(".Dup", obj_guid.ToString());

        //Create a duplicate of the object in the doc
        var copied_obj_guid = doc.Objects.Add(duplicate, attributes);

        //Create a new tag for the duplicate with updated functions
        if (!(ro.DuplicateGeometry() is TextEntity dup_text)) continue;
        dup_text.RichText = ReplaceGuid(dup_text.RichText, copied_obj_guid);
        var tag_guid = doc.Objects.Add(dup_text);

        //Select the new objects to make moving them easier
        Guid[] selection = { tag_guid, copied_obj_guid };
        doc.Objects.Select(selection);
        doc.Views.Redraw();

      }

      return Result.Success;
    }

    private static string GetNextIndexName(Guid originalGuid, ref RhinoDoc doc)
    {
      //Search for document objects that have an attribute value that matches the original object id
      var count = doc.Objects.Where(c => c.Attributes.GetUserString(".Dup") == originalGuid.ToString()).ToList().Count;
      var id = IntToLetters(count + 1);
      return id;

    }

    private static string IntToLetters(int value)
    {
      //Convert an int based index to A B C D index
      var result = string.Empty;
      while (--value >= 0)
      {
        result = (char)('A' + value % 26) + result;
        value /= 26;
      }
      return result;
    }

    private static string ExtractGuidFromString(string inputstring)
    {
      //Find the frist GUID nestled within a string
      var guids = Regex
          .Matches(inputstring,
              @"(\{){0,1}[0-9a-fA-F]{8}\-[0-9a-fA-F]{4}\-[0-9a-fA-F]{4}\-[0-9a-fA-F]{4}\-[0-9a-fA-F]{12}(\}){0,1}")
          .Cast<Match>().Select(m => m.Value);
      var enumerable = guids as string[] ?? guids.ToArray();
      return !enumerable.Any() ? string.Empty : enumerable.First();
    }

    private static string ReplaceGuid(string inputstring, Guid replacementguid)
    {
      //Replace a GUID or GUIDS within a string
      var guids = Regex
           .Matches(inputstring, @"(\{){0,1}[0-9a-fA-F]{8}\-[0-9a-fA-F]{4}\-[0-9a-fA-F]{4}\-[0-9a-fA-F]{4}\-[0-9a-fA-F]{12}(\}){0,1}")
           .Cast<Match>().Select(m => m.Value);
      var enumerable = guids as string[] ?? guids.ToArray();
      if (!enumerable.Any())
      {
        return inputstring;
      }

      var replace = inputstring;
      foreach (var match in enumerable)
        replace = inputstring.Replace(match, replacementguid.ToString());

      return replace;
    }

  }
}