//requires rhino3dm 8.0.0-beta3 and above

import * as fs from 'fs'
import rhino3dm from 'rhino3dm'

const rhino = await rhino3dm()
console.log('Loaded rhino3dm.')

// read model 
const model = '../../../models/Rhino_Logo_Mesh8_NamedViews.3dm'

const buffer = fs.readFileSync(model)
const arr = new Uint8Array(buffer)
const doc = rhino.File3dm.fromByteArray(arr)
const views = doc.views()
const namedviews = doc.namedViews()

console.log('--- Views ---')
for ( let i = 0; i < views.count; i ++ ) {
    const view = views.get(i)
    console.log(view.name)
    viewportInfo(view.getViewport())
}

console.log('--- Named Views ---')
for ( let i = 0; i < namedviews.count; i ++ ) {
    const view = namedviews.get(i)
    console.log(view.name)
    viewportInfo(view.getViewport())
}

function viewportInfo(vp) {
     console.log(`IsValidCameraFrame: ${vp.isValidCameraFrame}`)
     console.log(`IsValidCamera: ${vp.isValidCamera}`)
     console.log(`IsValidFrustum: ${vp.isValidFrustum}`)
     console.log(`IsParallelProjection: ${vp.isParallelProjection}`)
     console.log(`IsPerspectiveProjection: ${vp.isPerspectiveProjection}`)
     console.log(`IsTwoPointPerspectiveProjection: ${vp.isTwoPointPerspectiveProjection}`)
     console.log(`CameraLocation: ${vp.cameraLocation}`)
     console.log(`CameraDirection: ${vp.cameraDirection}`)
     console.log(`CameraUp: ${vp.cameraUp}`)
     console.log(`CameraX: ${vp.cameraX}`)
     console.log(`CameraY: ${vp.cameraY}`)
     console.log(`CameraZ: ${vp.cameraZ}`)
     console.log(`GetFrustum: ${vp.getFrustum()}`)
     console.log(`GetScreenPort: ${vp.screenPort}`)
     console.log(`ScreenPortAspect: ${vp.screenPortAspect}`)
     console.log(`CameraAngle: ${vp.cameraAngle}`)
     console.log(`Camera35mmLensLength: ${vp.camera35mmLensLength}`)
     console.log(`TargetPoint: ${vp.targetPoint}`)
     console.log(`Id: ${vp.id}`)
     console.log()
}