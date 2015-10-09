////////////////////////////////////////////////////////////////
// Copyright (c) 1993-2012 Robert McNeel & Associates. All rights reserved.
// OpenNURBS, Rhinoceros, and Rhino3D are registered trademarks of Robert
// McNeel & Assoicates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//				
// For complete openNURBS copyright information see <http://www.opennurbs.org>.
//
////////////////////////////////////////////////////////////////
//
//  example_read.cpp  
// 
//  Example program using the Rhino file IO toolkit.  The program reads in  
//  a Rhino 3dm model file and describes its contents.  The program is a 
//  console application that takes a filename as a command line argument.
//
////////////////////////////////////////////////////////////////////////
using System;

namespace example_read
{
  class Program
  {
    static void Main(string[] args)
    {
      if( args.Length < 1 )
      {
        PrintHelp();
        return;
      }

      string output_filename = null;
      bool chunk_dump = false;
      int maximum_directory_depth = 0;
      int file_count = 0;
      const bool bVerboseTextDump = true;

      Rhino.FileIO.TextLog dump = null;

      foreach( string arg in args )
      {
        // check for -out or /out option
        if( (arg.StartsWith("-out:") || arg.StartsWith("/out:")) && arg.Length > "-out:".Length )
        {
          // change destination of dump file
          output_filename = arg.Substring("-out:".Length).Trim();
          continue;
        }
        // check for -chunkdump or /chunkdump option
        if( arg.Equals("-c", StringComparison.OrdinalIgnoreCase) || arg.Equals("/c", StringComparison.OrdinalIgnoreCase) ||
            arg.Equals("-chunk", StringComparison.OrdinalIgnoreCase) || arg.Equals("/chunk", StringComparison.OrdinalIgnoreCase) ||
            arg.Equals("-chunkdump", StringComparison.OrdinalIgnoreCase) || arg.Equals("/chunkdump", StringComparison.OrdinalIgnoreCase) )
        {
          chunk_dump = true;
          continue;
        }
        // check for -recursive or /recursive option
        if( arg.Equals("-r", StringComparison.OrdinalIgnoreCase) || arg.Equals("/r", StringComparison.OrdinalIgnoreCase) ||
            arg.Equals("-recurse", StringComparison.OrdinalIgnoreCase) || arg.Equals("/recurse", StringComparison.OrdinalIgnoreCase) ||
            arg.Equals("-recursive", StringComparison.OrdinalIgnoreCase) || arg.Equals("/recursive", StringComparison.OrdinalIgnoreCase) )
        {
          maximum_directory_depth = 32;
          continue;
        }

        if( System.IO.Directory.Exists(arg) )
        {
          if (dump == null)
          {
            dump = new Rhino.FileIO.TextLog(output_filename);
            dump.IndentSize = 2;
          }

          file_count += ReadDirectoryHelper( 0, maximum_directory_depth, arg, null, bVerboseTextDump, chunk_dump, dump );
        }
        else
        {
          if (dump == null)
          {
            dump = new Rhino.FileIO.TextLog(output_filename);
            dump.IndentSize = 2;
          }

          if (ReadFileHelper(arg, bVerboseTextDump, chunk_dump, dump))
            file_count++;
        }
      }

      string example_read_exe_name = typeof(Program).Assembly.GetName().Name;
      string results = string.Format("{0} read {1} opennurbs model files.\n", example_read_exe_name, file_count);
      if (dump == null)
        Console.WriteLine(results);
      if (dump != null)
      {
        dump.Print(results);
        dump.Dispose();
      }
      Console.WriteLine("press any key to exit");
      Console.ReadKey();
    }


    static void PrintHelp()
    {
      string example_read_exe_name = typeof(Program).Assembly.GetName().Name;
      const string message = @"
SYNOPSIS:
  {0}.exe [-out:outputfilename.txt] [-c] [-r] <file or directory names>

DESCRIPTION:
  If a file is listed, it is read as an opennurbs model file.
  If a directory is listed, all .3dm files in that directory
  are read as opennurbs model files.

  Available options:
    -out:outputfilename.txt
      The output is written to the named file.
    -chunkdump
      Does a chunk dump instead of reading the file's contents.
    -recursive
      Recursivly reads files in subdirectories.

EXAMPLE:
  {0}.exe -out:list.txt -resursive .../example_files
  with read all the opennurbs .3dm files in the
  example_files/ directory and subdirectories.

Press any key to exit
";
      Console.Write(message, example_read_exe_name);
      Console.ReadKey();
    }


    /// <summary>
    /// </summary>
    /// <param name="filename">full name of file</param>
    /// <param name="dump"></param>
    /// <returns></returns>
    static bool Dump3dmFileHelper(string filename, Rhino.FileIO.TextLog dump)
    {
      dump.Print("====== FILENAME: {0}\n", filename);

      using (Rhino.FileIO.BinaryArchiveFile file = new Rhino.FileIO.BinaryArchiveFile(filename, Rhino.FileIO.BinaryArchiveMode.Read3dm))
      {
        if (!file.Open())
        {
          dump.Print("**ERROR** Unable to open file.\n");
          return false;
        }

        int version;
        string comment_block;
        if (!file.Reader.Read3dmStartSection(out version, out comment_block))
        {
          dump.Print("**ERROR** Read3dmStartSection() failed\n");
          return false;
        }
        
        dump.Print("====== VERSION: {0}\n",version );
        dump.Print("====== COMMENT BLOCK:\n");
        dump.Print(comment_block);
        dump.Print("====== CHUNKS:\n");
        
        while ( !file.Reader.AtEnd() )
        {
          uint typecode = file.Reader.Dump3dmChunk(dump);
          if( typecode==0 )
            break;
          if ( typecode == Rhino.FileIO.File3dmTypeCodes.TCODE_ENDOFFILE )
            break;
        }
        dump.Print("====== FINISHED: {0}\n",filename);
        file.Close();
        return true;
      }
    }


    // Returns: True if .3dm file was successfully read into a File3dmModel class.
    static bool ReadFileHelper(string filename, bool verboseTextDump, bool chunkDump, Rhino.FileIO.TextLog dump)
    {
      if (chunkDump)
        return Dump3dmFileHelper(filename, dump);

      dump.Print("\nOpenNURBS Archive File:  {0}\n", filename);

      // open file containing opennurbs archive
      Rhino.FileIO.File3dm model = Rhino.FileIO.File3dm.Read(filename);
      if (model == null)
      {
        dump.Print("  Unable to open file.\n");
        return false;
      }

      dump.PopIndent(); dump.PopIndent(); dump.PopIndent(); dump.PopIndent(); dump.PopIndent(); dump.PopIndent(); dump.PopIndent();

      dump.PushIndent();

      // print diagnostic
      dump.Print("Successfully read.\n");

      // see if everything is in good shape
      if( model.IsValid(dump) )
        dump.Print("Model is valid.\n");
      else
        dump.Print("Model is not valid.\n");

      // create a text dump of the model
      if (verboseTextDump)
      {
        dump.PushIndent();
        model.DumpToTextLog(dump);
        dump.PopIndent();
      }

      // destroy this model
      model.Dispose();

      dump.PopIndent();
      return true;
    }  

  
    // Returns: Number of files read
    static int ReadDirectoryHelper(int directoryDepth, int maxDirectoryDepth, string directoryName, string filenameFilter, bool verboseTextDump, bool chunkDump, Rhino.FileIO.TextLog dump)
    {
      int file_count = 0;

      if( directoryDepth <= maxDirectoryDepth )
      {
        if (string.IsNullOrWhiteSpace(filenameFilter))
          filenameFilter = "*.3dm";

        // read files in this directory
        foreach (string filename in System.IO.Directory.GetFiles(directoryName, filenameFilter))
        {
          if (ReadFileHelper(filename, verboseTextDump, chunkDump, dump))
            file_count++;
        }

        // read files in subdirectories
        foreach (string dir in System.IO.Directory.GetDirectories(directoryName))
        {
          file_count += ReadDirectoryHelper(directoryDepth + 1, maxDirectoryDepth, dir, filenameFilter, verboseTextDump, chunkDump, dump);
        }
      }
      return file_count;
    }
  }
}
