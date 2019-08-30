using System;
using System.Threading;

namespace SampleCsConsole
{
  class Program
  {
    static void Main()
    {
      // Try creating an instance of Rhino
      dynamic rhino = null;
      try
      {
        const string rhino_id = "Rhino.Application";
        var type = Type.GetTypeFromProgID(rhino_id);
        rhino = Activator.CreateInstance(type);
      }
      catch
      {
        // ignored
      }

      if (null == rhino)
      {
        Console.WriteLine("Failed to create Rhino application");
        return;
      }

      // Wait until Rhino is initialized before calling into it
      const int bail_milliseconds = 15 * 1000;
      var time_waiting = 0;
      while (0 == rhino.IsInitialized())
      {
        Thread.Sleep(100);
        time_waiting += 100;
        if (time_waiting > bail_milliseconds)
        {
          Console.WriteLine("Rhino initialization failed");
          return;
        }
      }

      rhino.Visible = 1;

      // SampleCsRhinoCommand is a hidden command in the SampleCsRhino plug-in
      // that basically does nothing. By running this do-nothing command,
      // we can make sure the SampleCsRhino plug-in is loaded before trying
      // to get it's scripting object.
      rhino.RunScript("_-SampleCsRhinoCommand", 0);

      // Try getting the SampleRhino plug-in's scripting object
      dynamic plugin = null;
      try
      {
        // Guid is from SampleCsRhino assembly
        const string plugin_id = "78d66fe1-1b04-4314-981c-5f260ad9c556";
        plugin = rhino.GetPlugInObject(plugin_id, plugin_id);
      }
      catch
      {
        // ignored
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
