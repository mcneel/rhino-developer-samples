using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.Windows.Forms;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;

namespace SampleCsCommands
{
  /// <summary>
  /// SampleCsCopyToClipboard command
  /// </summary>
  public class SampleCsCopyToClipboard : Command
  {
    public override string EnglishName => "SampleCsCopyToClipboard";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var sphere = new Sphere(Plane.WorldXY, 5.0);
      var brep = sphere.ToBrep();

      var rc = SampleCsGeometryBaseClipboardHelper.Write(brep);
      if (rc)
      {
        //Console.WriteLine("{0} copied to the Windows Clipboard", brep.ObjectType);
        RhinoApp.WriteLine("{0} copied to the Windows Clipboard", brep.ObjectType);
      }

      return Result.Success;
    }
  }

  /// <summary>
  /// SampleCsPasteFromClipboard command
  /// </summary>
  public class SampleCsPasteFromClipboard : Command
  {
    public override string EnglishName => "SampleCsPasteFromClipboard";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var geometry = SampleCsGeometryBaseClipboardHelper.Read();
      if (null == geometry)
      {
        RhinoApp.WriteLine("Nothing to paste.");
        return Result.Nothing;
      }

      doc.Objects.Add(geometry);
      doc.Views.Redraw();

      return Result.Success;
    }
  }

  public static class SampleCsGeometryBaseClipboardHelper
  {
    private const string CLIPBOARD_FORMAT = "Rhino.Geometry.GeometryBase";

    /// <summary>
    /// Writes an object, that inherits from Rhino.Geometry.GeometryBase to the Windows Clipboard.
    /// </summary>
    /// <param name="geometry">The geometry to write.</param>
    /// <returns>True if successful, false otherwise.</returns>
    public static bool Write(GeometryBase geometry)
    {
      if (null == geometry)
        return false;

      var bytes = GeometryBaseToBytes(geometry);
      if (null == bytes || 0 == bytes.Length)
        return false;

      return WriteClipboardBytes(bytes);
    }

    /// <summary>
    /// Reads an object, that inherits from Rhino.Geometry.GeometryBase from the Windows Clipboard.
    /// </summary>
    /// <returns>The object if successful, null otherwise.</returns>
    public static GeometryBase Read()
    {
      var bytes = ReadClipboardBytes();
      if (null == bytes || 0 == bytes.Length)
        return null;

      return BytesToGeometryBase(bytes);
    }

    /// <summary>
    /// Converts an object that inherits from Rhino.Geometry.GeometryBase to an array of bytes.
    /// </summary>
    private static byte[] GeometryBaseToBytes(GeometryBase src)
    {
      if (null == src)
        return null;

      var rc = new byte[0];
      try
      {
        var formatter = new BinaryFormatter();
        using (var stream = new MemoryStream())
        {
          formatter.Serialize(stream, src);
          rc = stream.ToArray();
        }
      }
      catch (Exception e)
      {
        Debug.WriteLine(e.Message);
      }

      return rc;
    }

    /// <summary>
    /// Converts an array of bytes to an object that inherits from Rhino.Geometry.GeometryBase.
    /// </summary>
    private static GeometryBase BytesToGeometryBase(byte[] bytes)
    {
      if (null == bytes || 0 == bytes.Length)
        return null;

      GeometryBase rc = null;
      try
      {
        using (var stream = new MemoryStream())
        {
          var formatter = new BinaryFormatter {Binder = new GeometryBaseBinder()};
          stream.Write(bytes, 0, bytes.Length);
          stream.Seek(0, SeekOrigin.Begin);
          if (formatter.Deserialize(stream) is GeometryBase geometry && geometry.IsValid)
            rc = geometry;
        }
      }
      catch (Exception e)
      {
        Debug.WriteLine(e.Message);
      }

      return rc;
    }

    /// <summary>
    /// Writes an array of bytes to the Window Clipboard in our format.
    /// </summary>
    private static bool WriteClipboardBytes(byte[] bytes)
    {
      if (null == bytes || 0 == bytes.Length)
        return false;

      var rc = false;
      try
      {
        var data = new DataObject();
        using (var stream = new MemoryStream())
        {
          stream.Write(bytes, 0, bytes.Length);
          data.SetData(CLIPBOARD_FORMAT, false, stream);
          Clipboard.SetDataObject(data, true);
          rc = true;
        }
      }
      catch (Exception e)
      {
        Debug.WriteLine(e.Message);
      }

      return rc;
    }

    /// <summary>
    /// Reads an array of bytes from the Window Clipboard in our format.
    /// </summary>
    private static byte[] ReadClipboardBytes()
    {
      if (!(Clipboard.GetDataObject() is DataObject data) || !data.GetDataPresent(CLIPBOARD_FORMAT))
        return null;

      var stream = data.GetData(CLIPBOARD_FORMAT) as MemoryStream;
      return stream?.ToArray();
    }

    /// <summary>
    /// Both RhinoCommon and Rhino3dmIO have a Rhino.Geometry.GeometryBase
    /// class. This serialization binder helps deserialize the equivalent 
    /// objects across the different assemblies.
    /// </summary>
    private class GeometryBaseBinder : SerializationBinder
    {
      public override Type BindToType(string assemblyName, string typeName)
      {
        if (assemblyName == null)
          throw new ArgumentNullException(nameof(assemblyName));

        var assembly = typeof(GeometryBase).Assembly;
        assemblyName = assembly.ToString();
        var type_to_deserialize = Type.GetType($"{typeName}, {assemblyName}");

        return type_to_deserialize;
      }
    }

  }
}