using Rhino.DocObjects;
using Rhino.FileIO;

File3dm file3dm = File3dm.Read("../../models/Rhino_Logo_Mesh8_NamedViews.3dm");

Console.WriteLine("--- Views ---");
for ( var i = 0; i < file3dm.AllViews.Count; i ++ ) {
    var view = file3dm.AllViews[i];
    Console.WriteLine(view.Name);
    ViewPortInfo(view.Viewport);
}

Console.WriteLine("--- Named Views ---");
for ( var i = 0; i < file3dm.AllNamedViews.Count; i ++ ) {
    var view = file3dm.AllNamedViews[i];
    Console.WriteLine(view.Name);
    ViewPortInfo(view.Viewport);
}

void ViewPortInfo(ViewportInfo vp) {

    //Console.WriteLine("IsValidCameraFrame: {0}",vp.IsIsValidCameraFrame);
    Console.WriteLine("IsValidCamera: {0}",vp.IsValidCamera);
    Console.WriteLine("IsValidFrustum: {0}",vp.IsValidFrustum);
    Console.WriteLine("IsParallelProjection: {0}",vp.IsParallelProjection);
    Console.WriteLine("IsPerspectiveProjection: {0}",vp.IsPerspectiveProjection);
    Console.WriteLine("IsTwoPointPerspectiveProjection: {0}",vp.IsTwoPointPerspectiveProjection);
    Console.WriteLine("CameraLocation: {0}",vp.CameraLocation);
    Console.WriteLine("CameraDirection: {0}",vp.CameraDirection);
    Console.WriteLine("CameraUp: {0}",vp.CameraUp);
    Console.WriteLine("CameraX: {0}",vp.CameraX);
    Console.WriteLine("CameraY: {0}",vp.CameraY);
    Console.WriteLine("CameraZ: {0}",vp.CameraZ);
    Console.WriteLine("Frustum Width: {0}",vp.FrustumWidth);
    Console.WriteLine("GetScreenPort: {0}",vp.ScreenPort);
    Console.WriteLine("ScreenPortAspect: {0}",vp.ScreenPortAspect);
    Console.WriteLine("CameraAngle: {0}",vp.CameraAngle);
    Console.WriteLine("Camera35mmLensLength: {0}",vp.Camera35mmLensLength);
    Console.WriteLine("TargetPoint: {0}",vp.TargetPoint);
    Console.WriteLine("Id: {0}",vp.Id);
    Console.WriteLine();

}
