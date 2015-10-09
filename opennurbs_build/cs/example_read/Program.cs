using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

// Adapted to C# from the C++ example_read sample found in the OpenNURBS source
// www.opennurbs.org

namespace example_read
{
  class Program
  {
    static void Main(string[] args)
    {
      if (args.Length < 1)
      {
        var exe_name = System.AppDomain.CurrentDomain.FriendlyName;
        Console.WriteLine("Syntax: {0} [-out:outputfilename.txt] file1.3dm file2.3dm ...", exe_name);
        return;
      }

      bool verbose_text_dump = false;
      System.IO.TextWriter alternate_output_stream = null;
      for (int i = 0; i < args.Length; i++)
      {
        string arg = args[i];
        // check for -out or /out option
        if (arg.StartsWith("-out:", StringComparison.InvariantCultureIgnoreCase) || arg.StartsWith("/out:", StringComparison.InvariantCultureIgnoreCase))
        {
          // change destination of dump file
          string dump_file_name = arg.Substring(5).Trim();
          alternate_output_stream = new System.IO.StreamWriter(dump_file_name);
          Console.SetOut(alternate_output_stream);
          continue;
        }

        string filename = arg;
        Console.Out.WriteLine("OpenNURBS Archive File:  {0}", filename);
        using (var model = Rhino.FileIO.File3dm.Read(filename))
        {
          if (model == null)
          {
            Console.Out.WriteLine("Unable to open file");
            continue;
          }

          // see if everything is in good shape
          string dump;
          if (model.IsValid(out dump))
          {
            Console.Out.WriteLine("Model is valid");
          }
          else
          {
            Console.Out.WriteLine("Model is not valid");
            Console.Out.Write(dump);
          }

          if (verbose_text_dump)
            dump = model.Dump();
          else
            dump = model.DumpSummary();
          dump = dump.Replace("\t", "   ");
          Console.Out.Write(dump);
        }
      }

      if (alternate_output_stream != null)
      {
        // close the text dump file
        alternate_output_stream.Close();
      }
    }
  }
}
