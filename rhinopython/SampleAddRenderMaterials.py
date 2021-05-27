################################################################################
# SampleAddRenderMaterials.py
# Copyright (c) 2018 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import System
import scriptcontext as sc

# Demonstrates how to create render materials
def SampleAddRenderMaterials():

    # The properties of a Rhino material define what is required to draw an object
    # using OpenGL. Most rendering plug-ins require a much richer definition than
    # this. And, not all rendering plug-ins support the same material properties.
    # This is where Render materials  come in. A Render material is an abstract
    # material - one that is defined by the owning rendering plug-in. But a basic
    # Render material does share some properties with a Rhino material.

    # This example demonstrates how to create a basic Render material. This material
    # will appear in Rhino's Materials panel. It it called a basic material because
    # it does not target any particular rendering plug-in.

    # 1.) Create a Rhino material.
    rhino_material = Rhino.DocObjects.Material()
    rhino_material.Name = "Burly"
    rhino_material.DiffuseColor = System.Drawing.Color.BurlyWood

    # 2.) Create a basic Render material from the Rhino material.
    render_material = Rhino.Render.RenderMaterial.CreateBasicMaterial(rhino_material)

    # 3.) Add a basic Render material to the document.
    sc.doc.RenderMaterials.Add(render_material)

    # At this point, you will see a Render material appear in Rhino's
    # Materials panel. Note, RhinoDoc.Materials.Count will equal 0. This
    # is because we have not added any Rhino materials. We've only added
    # a Render material. When you assign a Render material to an object
    # or a layer, a compatible Rhino material will be added RhinoDoc.Materials,
    # and this material will be referenced by the Render material.
 

    # NOTE: It is still possible to add plain Rhino materials to the document.
    # However, this is no longer recommended because Render materials should always be used.

    # 4.) Now we will create a sphere and assign the Render material to it.
    sphere = Rhino.Geometry.Sphere(Rhino.Geometry.Plane.WorldXY, 5.0)
    id = sc.doc.Objects.AddSphere(sphere);
    obj = sc.doc.Objects.Find(id);
    if obj != None:
        obj.RenderMaterial = render_material;
        obj.CommitChanges();

    sc.doc.Views.Redraw();

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleAddRenderMaterials()
