using Rhino;
using Rhino.Commands;
using Rhino.Input;
using System;
using System.IO;
using System.Windows.Forms;

namespace SampleCsCommands
{
  /// <summary>
  /// SampleCsEmbedTextFile
  /// </summary>
  public class SampleCsEmbedTextFile : Command
  {
    public override string EnglishName => "SampleCsEmbedTextFile";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      string path = null;
      if (mode == RunMode.Interactive)
      {
        OpenFileDialog dialog = new OpenFileDialog
        {
          Title = EnglishName,
          Filter = @"Text Documents|*.txt"
        };

        if (dialog.ShowDialog() != DialogResult.OK)
          return Result.Cancel;

        path = dialog.FileName;
      }
      else
      {
        Result rc = RhinoGet.GetString("Name of file to embed", false, ref path);
        if (rc != Result.Success)
          return rc;
      }

      path = path.Trim();
      if (string.IsNullOrEmpty(path))
        return Result.Nothing;

      if (!File.Exists(path))
      {
        RhinoApp.WriteLine("File not found.");
        return Result.Nothing;
      }

      string text;
      try
      {
        text = File.ReadAllText(path);
      }
      catch (Exception ex)
      {
        RhinoApp.WriteLine(ex.Message);
        return Result.Failure;
      }

      if (string.IsNullOrEmpty(text))
      {
        RhinoApp.WriteLine("File is empty.");
        return Result.Nothing;
      }

      string fname = Path.GetFileName(path);
      doc.Strings.SetString(fname, text);

      RhinoApp.Write("Embedded {0} ({1} characters)", fname, text.Length);

      return Result.Success;
    }
  }

  /// <summary>
  /// SampleCsExtractTextFile
  /// </summary>
  public class SampleCsExtractTextFile : Command
  {
    public override string EnglishName => "SampleCsExtractTextFile";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      string fname = null;
      Result rc = RhinoGet.GetString("Name of file to extract", false, ref fname);
      if (rc != Result.Success)
        return rc;

      fname = fname.Trim();
      if (string.IsNullOrEmpty(fname))
        return Result.Nothing;

      string text = doc.Strings.GetValue(fname);
      if (string.IsNullOrEmpty(text))
      {
        RhinoApp.WriteLine("File not found.");
        return Result.Nothing;
      }

      string path = null;
      if (mode == RunMode.Interactive)
      {
        SaveFileDialog dialog = new SaveFileDialog
        {
          Title = EnglishName,
          Filter = @"Text Documents|*.txt",
          FileName = fname
        };

        if (dialog.ShowDialog() != DialogResult.OK)
          return Result.Cancel;

        path = dialog.FileName;
      }
      else
      {
        rc = RhinoGet.GetString("Save file name", false, ref path);
        if (rc != Result.Success)
          return rc;
      }

      path = path.Trim();
      if (string.IsNullOrEmpty(path))
        return Result.Nothing;

      try
      {
        File.WriteAllText(path, text);
        // Optional, purge embedded text
        doc.Strings.Delete(fname);
      }
      catch (Exception ex)
      {
        RhinoApp.WriteLine(ex.Message);
        return Result.Failure;
      }

      return Result.Success;
    }
  }

}
