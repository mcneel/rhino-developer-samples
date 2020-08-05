using System;
using Rhino;
using Rhino.Geometry;
using Rhino.Render;
using System.Collections.Generic;

namespace SampleCustomLightManager
{
  /// <summary>
  /// The CustomLightManagerSupport class is an example of a custom lightmanager that
  /// includes all the lights in the document, but also contains one custom light
  /// that is not in the document. An instance of the class is created and registered
  /// automatically when the plugin is loaded
  /// </summary>
  public class CustomLightManager : Rhino.Render.LightManagerSupport
  {

    private Guid m_plugin_uuid;
    private Guid m_render_uuid;

    //private Rhino.Geometry.Light m_custom_light;
    private List<Rhino.Geometry.Light> m_light_array;
    /// <summary>
    /// The CustomLightManagerSupport constructor
    /// </summary>
    public CustomLightManager()
    {
      m_light_array = new List<Rhino.Geometry.Light>();

      // The Guid of the plugin
      m_plugin_uuid = new Guid("0d54f97c-fe13-4d6f-86d8-960fd4c8c5de");

      // The Guid of the render engine that the lightmanger represents
      m_render_uuid = new Guid("0d54f97c-fe13-4d6f-86d8-960fd4c8c5de");

      // Create first custom light
      CustomLight custom_light_1 = new CustomLight();
      custom_light_1.Name = "Custom Light 1";
      custom_light_1.Intensity = 0.26;
      custom_light_1.IsEnabled = true;
      custom_light_1.CustomLightDescription = "Disco ball";
      // all custom lights should have unique ids
      custom_light_1.Id = new Guid("5115C78B-4483-4F5F-BEE6-9B35637A7F6B");

      // Create second custom light
      CustomLight custom_light_2 = new CustomLight();
      custom_light_2.Name = "Custom Light 2";
      custom_light_2.Intensity = 0.75;
      custom_light_2.IsEnabled = true;
      custom_light_2.CustomLightDescription = "Studio light";
      // all custom lights should have unique ids
      custom_light_2.Id = new Guid("8F6CE74C-ACEF-4DB9-AA07-374F615FD073");

      // Add all custom lights to internal lights array
      m_light_array.Add(custom_light_1);
      m_light_array.Add(custom_light_2);
    }

    /// <summary>
    /// Delete the light
    /// </summary>
    public override bool DeleteLight(RhinoDoc doc, Light light, bool bUndelete)
    {
      int index = doc.Lights.Find(light.Id, false);
      if (index > -1)
      {
        return doc.Lights.Delete(index, true);
      }
      else
      {
        for (int i = 0; i < m_light_array.Count; i++)
        {
          Rhino.Geometry.Light custom_light = m_light_array[i];
          if (light.Id == custom_light.Id)
          {
            m_light_array.RemoveAt(i);

            // In case of custom lights, we need to trigger our own event that triggers
            // a light table event so that the lightmanager will be notified.
            OnCustomLightEvent(doc, LightMangerSupportCustomEvent.light_deleted, ref light);

            return true;
          }
        }
      }
      return false;
    }

    /// <summary>
    /// Return all the lights in the lightmanager
    /// </summary>
    public override void GetLights(RhinoDoc doc, ref LightArray light_array)
    {
      // Append all lights in doc
      for (int i = 0; i < doc.Lights.Count; i++)
      {
        Rhino.DocObjects.LightObject light = doc.Lights[i];
        if (!light.IsDeleted)
          light_array.Append(light.LightGeometry);
      }

      // Append also the custom light
      foreach (Rhino.Geometry.Light custom_light in m_light_array)
      {
        light_array.Append(custom_light);
      }
    }

    /// <summary>
    /// Return a description of the light
    /// </summary>
    public override string LightDescription(RhinoDoc doc, ref Light light)
    {
      string description = "";
      // Check if light is in doc and return it
      int index = doc.Lights.Find(light.Id, true);
      if (index > -1)
      {
        if (light.IsPointLight)
        {
          description = "Point";
        }
        else
      if (light.IsDirectionalLight)
        {
          description = "Directional";
        }
        else
      if (light.IsLinearLight)
        {
          description = "Linear";
        }
        else
      if (light.IsRectangularLight)
        {
          description = "Rectangular";
        }
        else
      if (light.IsSpotLight)
        {
          description = "Spotlight";
        }
      }
      else
      {
        // If not in doc, check if light matches custom light
        foreach (Rhino.Geometry.Light c_light in m_light_array)
        {
          if (c_light.Id == light.Id)
          {
            CustomLight custom_light = (CustomLight)c_light;
            return custom_light.CustomLightDescription;
          }
        }
      }
      return description;
    }

    /// <summary>
    /// Get the Light from id
    /// </summary>
    public override bool LightFromId(RhinoDoc doc, Guid uuid, ref Light rLight)
    {
      // Check if light is in doc and return it
      int index = doc.Lights.Find(uuid, true);
      if (index > -1)
      {
        Rhino.Geometry.Light light_geometry = doc.Lights[index].LightGeometry;
        rLight = light_geometry;
        return true;
      }
      else
      {
        // If not in doc, check if light matches custom light and return it
        foreach (Rhino.Geometry.Light custom_light in m_light_array)
        {
          if (custom_light.Id == uuid)
          {
            rLight = custom_light;
            return true;
          }
        }
      }
      return false;
    }

    /// <summary>
    /// Modify light
    /// </summary>
    public override void ModifyLight(RhinoDoc doc, Light light)
    {
      // If light is in doc, we modify the light and the light
      // table will generate events
      int modifyed_index = doc.Lights.Find(light.Id, true);
      if (modifyed_index > -1)
      {
        Rhino.Geometry.Light light_geometry = doc.Lights[modifyed_index].LightGeometry;
        light_geometry.Intensity = light.Intensity;
        light_geometry.Diffuse = light.Diffuse;
        light_geometry.IsEnabled = light.IsEnabled;
        light_geometry.Name = light.Name;
        doc.Lights.Modify(light.Id, light_geometry);
      }
      else
      {
        // If not in doc, check if light matches custom light and
        // modify custom light.
        foreach (Rhino.Geometry.Light custom_light in m_light_array)
        {
          if (light.Id == custom_light.Id)
          {
            custom_light.Intensity = light.Intensity;
            custom_light.Diffuse = light.Diffuse;
            custom_light.IsEnabled = light.IsEnabled;
            custom_light.Name = light.Name;

            // In case of custom lights, we need to trigger our own event that triggers
            // a light table event so that the lightmanager will be notified.
            OnCustomLightEvent(doc, LightMangerSupportCustomEvent.light_modified, ref light);
          }
        }
      }

    }

    /// <summary>
    /// Get the object serial number of the light
    /// </summary>
    public override int ObjectSerialNumberFromLight(RhinoDoc doc, ref Light light)
    {
      int index = doc.Lights.Find(light.Id, true);
      if (index > -1)
      {
        Rhino.DocObjects.LightObject light_obj = doc.Lights[index];
        return (int)light_obj.RuntimeSerialNumber;
      }
      return 0;
    }

    /// <summary>
    /// Selects the lights and opens the Light Properties page
    /// </summary>
    public override bool OnEditLight(RhinoDoc doc, ref LightArray light_array)
    {
#pragma warning disable CS0162 // Unreachable code detected
       for (int i = 0; i < light_array.Count(); i++)
#pragma warning restore CS0162 // Unreachable code detected
       {
        Rhino.Geometry.Light light = light_array.ElementAt(i);
        int index = doc.Lights.Find(light.Id, true);
        if (index > -1)
        {
          Rhino.DocObjects.LightObject light_obj = doc.Lights[index];
          light_obj.Select(true);
          RhinoApp.RunScript("_-PropertiesPage _Light", true);
        }
        return true;
      }
      return false;
    }

    /// <summary>
    /// Get the plugin Guid
    /// </summary>
    public override Guid PluginId()
    {
      return m_plugin_uuid;
    }

    /// <summary>
    /// Get the render engine Guid for the lightmanager
    /// </summary>
    public override Guid RenderEngineId()
    {
      return m_render_uuid;
    }

    // ToDo: The Solo functionality is not yet implemented in the Sample project
    public override bool SetLightSolo(RhinoDoc doc, Guid uuid_light, bool bSolo)
    {
      return false;
    }

    public override bool GetLightSolo(RhinoDoc doc, Guid uuid_light)
    {
      return false;
    }

    public override int LightsInSoloStorage(RhinoDoc doc)
    {
      return 0;
    }

    public override void GroupLights(RhinoDoc doc, ref LightArray light_array)
    {
      
    }

    public override void UnGroup(RhinoDoc doc, ref LightArray light_array)
    {
      
    }
  }

  public class CustomLight : Rhino.Geometry.Light
  {
    private string m_custom_description;

    public CustomLight()
    {
      m_custom_description = "";
    }

    public string CustomLightDescription
    {
      get
      {
        return m_custom_description;
      }

      set
      {
        m_custom_description = value;
      }
    }
  }
}
