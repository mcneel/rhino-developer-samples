import rhino3dm
model = rhino3dm.File3dm.Read('../models/Rhino_Logo_Mesh8_NamedViews.3dm')

def viewportInfo(vp):
    print("IsValidCameraFrame: " + str(vp.IsValidCameraFrame))
    print("IsValidCamera: " + str(vp.IsValidCamera))
    print("IsValidFrustum: " + str(vp.IsValidFrustum))
    print("IsParallelProjection: " + str(vp.IsParallelProjection))
    print("IsPerspectiveProjection: " + str(vp.IsPerspectiveProjection))
    print("IsTwoPointPerspectiveProjection: " + str(vp.IsTwoPointPerspectiveProjection))
    print("CameraLocation: " + str(vp.CameraLocation))
    print("CameraDirection: " + str(vp.CameraDirection))
    print("CameraUp: " + str(vp.CameraUp))
    print("CameraX: " + str(vp.CameraX))
    print("CameraY: " + str(vp.CameraY))
    print("CameraZ: " + str(vp.CameraZ))
    print("GetFrustum: " + str(vp.GetFrustum))
    print("GetScreenPort: " + str(vp.GetScreenPort))
    print("ScreenPortAspect: " + str(vp.ScreenPortAspect))
    print("CameraAngle: " + str(vp.CameraAngle))
    print("Camera35mmLensLength: " + str(vp.Camera35mmLensLength))
    print("TargetPoint: " + str(vp.TargetPoint))
    print("Id: " + str(vp.Id))
    print("")

print("--- Views ---")  
for view in model.Views:
    print(view.Name)
    viewportInfo(view.Viewport)

print("--- Named Views ---") 
for namedView in model.NamedViews:
    print(namedView.Name)
    viewportInfo(namedView.Viewport)