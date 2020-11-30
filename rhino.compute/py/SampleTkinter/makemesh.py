from rhino3dm import *
import tkinter as Tkinter
import time
import compute_rhino3d.Mesh
import compute_rhino3d.Util

auth_token = ""
top = Tkinter.Tk()
def onClickOk():
    global auth_token
    auth_token = auth.get()
    top.destroy()
lbl = Tkinter.Label(top, text="Rhino Accounts auth token")
lbl.grid(row=0, column=0)
auth = Tkinter.Entry(top)
auth.grid(row=0, column=1)
btn = Tkinter.Button(top, text='Ok', command=onClickOk)
btn.grid(row=1, column=1)
top.mainloop()

compute_rhino3d.Util.authToken = auth_token
center = Point3d(250, 250, 0)
sphere = Sphere(center, 100)

brep = sphere.ToBrep()
d = compute_rhino3d.Mesh.CreateFromBrep(brep)
mesh = (d[0])

top = Tkinter.Tk()

w = Tkinter.Canvas(top, width=500, height=500)
w.pack()

v = Vector3d(1,1,0)
for x in range(100):
    top.update()
    time.sleep(0.01)
    w.delete("all")
    mesh.Rotate(.1, v, center)
    verts = mesh.Vertices
    faces = mesh.Faces
    for i in range(len(faces)):
        face = faces[i]
        pts = [verts[face[0]], verts[face[1]], verts[face[2]], verts[face[3]]]
        w.create_line(pts[0].X, pts[0].Y, pts[1].X, pts[1].Y)
        w.create_line(pts[1].X, pts[1].Y, pts[2].X, pts[2].Y)
        w.create_line(pts[2].X, pts[2].Y, pts[3].X, pts[3].Y)
        w.create_line(pts[3].X, pts[3].Y, pts[0].X, pts[0].Y)

top.mainloop()
