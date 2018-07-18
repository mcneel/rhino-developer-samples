################################################################################
# SampleAddRenderMaterials.py
# Copyright (c) 2018 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import System
import scriptcontext as sc

# Demonstrates how to creaste render materials
def SampleAddRenderMaterials():
    
    # Properties of a Rhino material define what is required to draw an
    # object using OpenGL. Most rendering plug-ins require a much richer
    # defintion than this. And, not all rendering  plug-ins support the
    # same material properties. This is where Render materials come in. 
    
    # A Render material is an abstract material - one that is defined 
    # by the the owning render plug-in. But a basic Render material does
    # share some properties with a Rhino material.
    
    # This example demonstrates how to create a basic Render material.
    # This material will appear in Rhino's Materials panel. It it called
    # basic material because it does not target any particular rendering
    # plug-in. 
    
    # 1.) Create some Rhino materials
    red_material = Rhino.DocObjects.Material()
    red_material.DiffuseColor = System.Drawing.Color.Red
    red_material.Name = "Red"
    
    green_material = Rhino.DocObjects.Material()
    green_material.DiffuseColor = System.Drawing.Color.Green
    green_material.Name = "Green"

    blue_material = Rhino.DocObjects.Material()
    blue_material.DiffuseColor = System.Drawing.Color.Blue
    blue_material.Name = "Blue"
    
    # 2.) Create basic Render materials from Rhino materials
    red_render_material = Rhino.Render.RenderMaterial.CreateBasicMaterial(red_material)
    green_render_material = Rhino.Render.RenderMaterial.CreateBasicMaterial(green_material)
    blue_render_material = Rhino.Render.RenderMaterial.CreateBasicMaterial(blue_material)
    
    # 3.) Add basic Render materials to the RDK document
    sc.doc.RenderMaterials.Add(red_render_material)
    sc.doc.RenderMaterials.Add(green_render_material)
    sc.doc.RenderMaterials.Add(blue_render_material)
    
    # At this point, you will see render materials show up in Rhino's
    # Materials panel. Note, RhinoDoc.Materials.Count will equal 0. This
    # is because we have not added any Rhino materials. We've only added
    # Render materials. When you assign a Render material to an object
    # or a layer, a compatible Rhino material will be added RhinoDoc.Materials,
    # and this material will be referenced by the Render material.
    sc.doc.Layers[0].RenderMaterial = blue_render_material;  
    
    # Note, it is possible to add Rhino materials to the document. However,
    # they will not appear in the Materials panel until they are assigned 
    # to objects or layers. Here is an example of this.
    
    # 1.) Create a Rhino material
    magenta_material = Rhino.DocObjects.Material()
    magenta_material.DiffuseColor = System.Drawing.Color.Magenta
    magenta_material.Name = "Magenta"

    # 2.) Add the Rhino material to the document
    magenta_material_index = sc.doc.Materials.Add(magenta_material)

    # 3.) Create an object and assign the Rhino material to it.
    sphere = Rhino.Geometry.Sphere(Rhino.Geometry.Plane.WorldXY, 5.0)
    attributes = sc.doc.CreateDefaultAttributes()
    attributes.MaterialSource = Rhino.DocObjects.ObjectMaterialSource.MaterialFromObject
    attributes.MaterialIndex = magenta_material_index
    sc.doc.Objects.AddSphere(sphere, attributes)
    sc.doc.Views.Redraw()

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleAddRenderMaterials()
