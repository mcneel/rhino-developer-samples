using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Linq;
using System.Text;
using System.Threading;

namespace SampleCommand
{
  class Program
  {
    static void Main(string[] args)
    {
      // Try creating an instance of Rhino
      dynamic rhino = null;
      try
      {
        // string rhinoId = "Rhino5.Application";
        string rhinoId = "Rhino5x64.Application";
        System.Type type = System.Type.GetTypeFromProgID(rhinoId);
        rhino = System.Activator.CreateInstance(type);
      }
      catch
      {
      }

      if (null == rhino)
      {
        Console.WriteLine("Failed to create Rhino application");
        return;
      }

      // Wait until Rhino is initialized before calling into it
      const int bail_milliseconds = 15 * 1000;
      int time_waiting = 0;
      while( 0 == rhino.IsInitialized() )
      {
        Thread.Sleep( 100 );
        time_waiting += 100;
        if( time_waiting > bail_milliseconds )
        {
          Console.WriteLine("Rhino initialization failed");
          return;
        }
      }

      // SampleRhinoCommand is a hidden command in the SampleRhino plug-in
      // that basically does nothing. By running this do-nothing command,
      // we can make sure the SampleRhino plug-in is loaded before trying
      // to get it's scripting object.
      rhino.RunScript("_-SampleRhinoCommand", 0);

      // Try getting the SampleRhino plug-in's scripting object
      dynamic plugin = null;
      try
      {
        string pluginId = "593f9f2c-811e-46ad-8277-720cc2b386a6";
        plugin = rhino.GetPlugInObject(pluginId, pluginId);
      }
      catch
      {
      }

      if (null == plugin)
      {
        Console.WriteLine("Failed to get SampleRhino plug-in object");
        return;
      }

      // Call a few methods on the scripting object...

      int n = plugin.GetInteger();
      Console.WriteLine("SampleRhino integer = {0}", n);

      double d = plugin.GetDouble();
      Console.WriteLine("SampleRhino double = {0}", d);

      string s = plugin.GetString();
      Console.WriteLine("SampleRhino string = {0}", s);

      Console.WriteLine("Press any key to continue...");
      Console.ReadKey();
    }
  }
}
