using System;
using System.Collections.Generic;
using Rhino.Render;
using Rhino.Render.Fields;

namespace SampleRdkMaterialAutoUI
{
  // .sample example file
  //
  // syntax type=<type>,key=<key>,value=<True/False>,prompt=<UI text here>
  //
  // One example
  // ***********
  //
  // type=bool,key=bool,value=False,prompt=Yes/No
  // type = bool, key = bool2, value = False, prompt = Yes2 / No2
  // type=bool,key=color-texture-on,value=True,prompt=Color texture on

  // This class is our custom serializer for .sample material files.
  // The file extension .sample and it is able to load and save .sample
  // files. The sample project saves and loads only boolean fields.
  class SampleRenderContentSerializer : RenderContentSerializer
  {
    public SampleRenderContentSerializer(string fileExtension, RenderContentKind contentKind, bool canRead, bool canWrite) : base(fileExtension, contentKind, canRead, canWrite)
    {
    }

    public override string EnglishDescription
    {
      get { return "Sample Render Content Serializer"; }
    }

    public override RenderContent Read(string pathToFile)
    {
      // Create new Sample Material without fields.
      // We will read the fields from the file and
      // add them to the material
      var material = new SampleRdkMaterial();

      // Read values from file and set values
      string line;
      var file = new System.IO.StreamReader(pathToFile);
      while ((line = file.ReadLine()) != null)
      {
        string[] type_prompt_key_and_value = line.Split(',');
        if (type_prompt_key_and_value.Length == 4)
        {
          string[] type_value = type_prompt_key_and_value[0].Split('=');
          if (type_value.Length == 2)
          {
            // Get field type
            string type = type_value[1];

            // Check if bool field
            if (type.Equals("bool"))
            {
              string[] key_and_value = type_prompt_key_and_value[1].Split('=');
              if (key_and_value.Length == 2)
              {
                // Get field key
                string key = key_and_value[1];

                string[] value_text = type_prompt_key_and_value[2].Split('=');
                if (value_text.Length == 2)
                {
                  // Get field value
                  string value = value_text[1];
                  Boolean.TryParse(value, out bool bool_value);

                  string[] prompt_value = type_prompt_key_and_value[3].Split('=');
                  if (prompt_value.Length == 2)
                  {
                    // Get field prompt (text shown in UI)
                    string prompt = prompt_value[1];

                    // Finally add the bool field
                    material.Fields.Add(key, bool_value, prompt);
                  }
                }
              }
            }
          }
        }
      }

      file.Close();

      // Return material to rdk
      return material;
    }

    // Write sample material to .sample file (only bool values)
    public override bool Write(string pathToFile, RenderContent renderContent, CreatePreviewEventArgs previewArgs)
    {
      var lines = new List<string>();

      foreach (var field in renderContent.Fields)
      {
        var type = field.GetType();

        // Bool fields
        if (type == typeof(BoolField))
        {
          string key = field.Name;
          string prompt = field.Name;
          renderContent.Fields.TryGetValue(key, out bool bool_value);
          string value = bool_value.ToString();
          lines.Add("type=bool," + "key=" + key + ",value=" + value + "," + "prompt=" + prompt);
        }
      }

      // Write all lines to .sample file
      using (var file = new System.IO.StreamWriter(pathToFile))
      {
        foreach (string line in lines)
        {
          file.WriteLine(line);
        }

        file.Close();
      }

      return true;
    }
  }
}
