using System;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Runtime.Serialization.Formatters.Binary;
using Rhino.Geometry;

namespace SampleCsDeserializeEmbeddedResource
{
  public static class SampleCsGeometryHelper
  {
    /// <summary>
    /// Writes a GeometryBase-derived object to a file.
    /// </summary>
    public static bool WriteToFile(string path, GeometryBase geometry)
    {
      if (string.IsNullOrEmpty(path))
        return false;

      if (null == geometry)
        return false;

      var bytes = ToBytes(geometry);
      if (null == bytes || 0 == bytes.Length)
        return false;

      bool rc;
      try
      {
        using (var stream = new FileStream(path, FileMode.Create, FileAccess.Write))
        {
          stream.Write(bytes, 0, bytes.Length);
          rc = true;
        }
      }
      catch (Exception e)
      {
        Debug.WriteLine(e.Message);
        rc = false;
      }

      return rc;
    }

    /// <summary>
    /// Reads a GeometryBase-derived object from an embedded resource.
    /// </summary>
    public static GeometryBase ReadFromEmbeddedResource(string resource)
    {
      if (string.IsNullOrEmpty(resource))
        return null;

      try
      {
        var assembly = Assembly.GetExecutingAssembly();
        using (var stream = assembly.GetManifestResourceStream(resource))
        {
          if (null != stream)
          {
            var bytes_to_read = (int)stream.Length;
            if (bytes_to_read > 0)
            {
              var bytes = new byte[bytes_to_read];
              var bytes_read = stream.Read(bytes, 0, bytes_to_read);
              if (bytes_read == bytes_to_read)
              {
                var geometry = ToGeometryBase(bytes);
                return geometry;
              }
            }
          }
        }
      }
      catch (Exception e)
      {
        Debug.WriteLine(e.Message);
      }

      return null;
    }

    /// <summary>
    /// Converts an object that inherits from GeometryBase to an array of bytes.
    /// </summary>
    private static byte[] ToBytes(GeometryBase src)
    {
      var rc = new byte[0];

      if (null == src)
        return rc;

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
    /// Creates an object that inherits from GeometryBase from an array of bytes.
    /// </summary>
    private static GeometryBase ToGeometryBase(byte[] bytes)
    {
      if (null == bytes || 0 == bytes.Length)
        return null;

      GeometryBase rc = null;
      try
      {
        using (var stream = new MemoryStream())
        {
          var formatter = new BinaryFormatter();
          stream.Write(bytes, 0, bytes.Length);
          stream.Seek(0, SeekOrigin.Begin);
          var geometry = formatter.Deserialize(stream) as GeometryBase;
          if (null != geometry && geometry.IsValid)
            rc = geometry;
        }
      }
      catch (Exception e)
      {
        Debug.WriteLine(e.Message);
      }

      return rc;
    }
  }
}
