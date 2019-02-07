using System;
using System.Collections.Generic;
using System.IO;
using GH_IO.Serialization;

namespace GhFileAnalysis
{
  internal class Program
  {
    private static void Main(string[] args)
    {
      string uri = string.Empty;
      if (args.Length == 1)
        uri = args[0];

      while (!File.Exists(uri))
      {
        WriteLine(ConsoleColor.Green, "Type the uri of the file to load.");
        uri = Console.ReadLine();
      }

      var archive = ReadFile(uri);
      if (archive is null)
        return;

      Console.WriteLine();
      while (true)
      {
        WriteLine(ConsoleColor.DarkGreen, $"File '{Path.GetFileName(uri)}' read, what data would you like to display?");
        WriteLine(ConsoleColor.DarkGreen, "A = Author data");
        WriteLine(ConsoleColor.DarkGreen, "F = File data");
        WriteLine(ConsoleColor.DarkGreen, "N = Object count");
        WriteLine(ConsoleColor.DarkGreen, "P = Plugin list");
        WriteLine(ConsoleColor.DarkGreen, "Q = Quit");
        Console.WriteLine();
        var key = Console.ReadKey(true);

        switch (key.Key)
        {
          case ConsoleKey.Escape:
          case ConsoleKey.Q:
            return;

          case ConsoleKey.A:
            DisplayAuthorStatistics(archive);
            break;

          case ConsoleKey.F:
            DisplayDocumentStatistics(archive);
            break;

          case ConsoleKey.N:
            DisplayObjectStatistics(archive);
            break;

          case ConsoleKey.P:
            DisplayPluginStatistics(archive);
            break;

          default:
            WriteLine(ConsoleColor.Red, $"{key.Key} is not a recognised command.");
            break;
        }

        Console.WriteLine();
        Console.WriteLine("Press any key to return to the prompt...");
        Console.ReadKey(true);
        Console.Clear();
      }
    }

    private static GH_Archive ReadFile(string path)
    {
      var archive = new GH_Archive();
      try
      {
        if (archive.ReadFromFile(path))
        {
          if (archive.MessageCount() > 0)
            foreach (var msg in archive.Messages)
              switch (msg.Type)
              {
                case GH_Message_Type.warning:
                  WriteLine(ConsoleColor.DarkYellow, "  " + msg.Message);
                  break;

                case GH_Message_Type.error:
                  WriteLine(ConsoleColor.Red, "  " + msg.Message);
                  break;
              }

          if (archive.GetRootNode is null)
          {
            WriteLine(ConsoleColor.Red, "An error occured while reading that file:");
            WriteLine(ConsoleColor.Red, "  The archive does not contain a root node.");
            Console.ReadKey(true);
            return null;
          }
          return archive;
        }

        Console.ReadKey(true);
        return null;
      }
      catch (Exception ex)
      {
        WriteLine(ConsoleColor.Red, "An error occured while reading that file:");
        WriteLine(ConsoleColor.Red, "  " + ex.Message);
        Console.ReadKey(true);
        return null;
      }
    }
    private static void WriteLine(ConsoleColor colour, string line)
    {
      var current = Console.ForegroundColor;
      Console.ForegroundColor = colour;
      Console.WriteLine(line);
      Console.ForegroundColor = current;
    }

    private static void DisplayAuthorStatistics(GH_Archive archive)
    {
      var root = archive.GetRootNode;
      var definition = root.FindChunk("Definition");
      var author = definition.FindChunk("Author");

      if (author is null)
      {
        WriteLine(ConsoleColor.Red, "File does not contain any author data.");
        WriteLine(ConsoleColor.Red, "This is not indicative of a problem as it is optional.");
        return;
      }

      string name = "<unspecified>";
      string email = "<unspecified>";
      string phone = "<unspecified>";
      string address = "<unspecified>";
      string website = "<unspecified>";
      string company = "<unspecified>";
      string copyright = "<unspecified>";

      author.TryGetString("Name", ref name);
      author.TryGetString("EMail", ref email);
      author.TryGetString("Phone", ref phone);
      author.TryGetString("Address", ref address);
      author.TryGetString("Website", ref website);
      author.TryGetString("Company", ref company);
      author.TryGetString("Copyright", ref copyright);

      Console.Write("Name:      "); WriteLine(ConsoleColor.DarkCyan, name);
      Console.Write("Company:   "); WriteLine(ConsoleColor.DarkCyan, company);
      Console.Write("Copyright: "); WriteLine(ConsoleColor.DarkCyan, copyright);
      Console.Write("E-mail:    "); WriteLine(ConsoleColor.DarkCyan, email);
      Console.Write("Phone:     "); WriteLine(ConsoleColor.DarkCyan, phone);
      Console.Write("Website:   "); WriteLine(ConsoleColor.DarkCyan, website);
      Console.Write("Address:   "); WriteLine(ConsoleColor.DarkCyan, address.Replace(Environment.NewLine, ", "));
    }
    private static void DisplayDocumentStatistics(GH_Archive archive)
    {
      var root = archive.GetRootNode;

      var definition = root.FindChunk("Definition");
      var header = definition.FindChunk("DocumentHeader");
      var properties = definition.FindChunk("DefinitionProperties");

      var archiveVersion = root.GetVersion("ArchiveVersion");
      var pluginVersion = definition.GetVersion("plugin_version");
      var documentId = header.GetGuid("DocumentID");
      var documentDate = properties.GetDate("Date");
      var documentName = properties.GetString("Name");

      Console.Write("Archive Version: "); WriteLine(ConsoleColor.DarkCyan, archiveVersion.ToString());
      Console.Write("Plug-In Version: "); WriteLine(ConsoleColor.DarkCyan, pluginVersion.ToString());
      Console.Write("Document ID:     "); WriteLine(ConsoleColor.DarkCyan, $"{{{documentId}}}");
      Console.Write("Document Date:   "); WriteLine(ConsoleColor.DarkCyan, documentDate.ToLongDateString() + " " + documentDate.ToLongTimeString());
      Console.Write("Document Name:   "); WriteLine(ConsoleColor.DarkCyan, $"'{documentName}'");
    }
    private static void DisplayObjectStatistics(GH_Archive archive)
    {
      var root = archive.GetRootNode;
      var definition = root.FindChunk("Definition");
      var objects = definition.FindChunk("DefinitionObjects");

      var count = objects.GetInt32("ObjectCount");
      var missing = 0;

      var guids = new List<Guid>(count);
      var plurality = new List<int>(count);
      var names = new List<string>(count);

      for (int i = 0; i < count; i++)
      {
        var chunk = objects.FindChunk("Object", i);
        if (chunk is null)
          missing++;
        else
          ParseObjectChunk(chunk, guids, plurality, names);
      }

      var descriptors = new string[guids.Count];
      for (int i = 0; i < guids.Count; i++)
      {
        var prefix = $"{plurality[i]} × {names[i]} ";
        if (prefix.Length < 30)
          prefix += new string(' ', 30 - prefix.Length);
        descriptors[i] = $"{prefix}({guids[i]})";
      }
      Array.Sort(plurality.ToArray(), descriptors);
      Array.Reverse(descriptors);

      Console.Write("Object Count:   "); WriteLine(ConsoleColor.DarkCyan, count.ToString());
      Console.Write("Missing Chunks: "); WriteLine(ConsoleColor.DarkRed, missing.ToString());

      for (int i = 0; i < descriptors.Length; i++)
        WriteLine(ConsoleColor.Magenta, descriptors[i]);
    }
    private static void ParseObjectChunk(GH_IReader chunk, List<Guid> guids, List<int> plurality, List<string> names)
    {
      var id = chunk.GetGuid("GUID");
      var name = chunk.GetString("Name");

      var index = guids.IndexOf(id);
      if (index >= 0)
        plurality[index]++;
      else
      {
        guids.Add(id);
        names.Add(name);
        plurality.Add(1);
      }
    }
    private static void DisplayPluginStatistics(GH_Archive archive)
    {
      var root = archive.GetRootNode;
      var definition = root.FindChunk("Definition");
      var libraries = definition.FindChunk("GHALibraries");

      if (libraries is null)
      {
        WriteLine(ConsoleColor.DarkRed, "Grasshopper file contains no plug-in library data...");
        return;
      }

      var count = libraries.GetInt32("Count");
      WriteLine(ConsoleColor.DarkCyan, $"Grasshopper file contains components from {count} non-standard plug-ins.");

      for (int i = 0; i < count; i++)
      {
        WriteLine(ConsoleColor.DarkCyan, $"Plugin {i + 1}:");
        var chunk = libraries.FindChunk("Library", i);
        DisplayPluginStatistics(chunk);
        Console.WriteLine();
      }
    }
    private static void DisplayPluginStatistics(GH_IReader chunk)
    {
      if (chunk is null)
      {
        WriteLine(ConsoleColor.Red, "Missing plug-in record in file.");
        return;
      }

      var id = chunk.GetGuid("Id");
      var name = chunk.GetString("Name");
      var author = chunk.GetString("Author");
      var version = chunk.GetString("Version");

      Console.Write("  Name:    "); WriteLine(ConsoleColor.DarkCyan, name);
      Console.Write("  Author:  "); WriteLine(ConsoleColor.DarkCyan, author);
      Console.Write("  Version: "); WriteLine(ConsoleColor.DarkCyan, version);
      Console.Write("  ID:      "); WriteLine(ConsoleColor.DarkCyan, $"{{{id}}}");
    }
  }
}