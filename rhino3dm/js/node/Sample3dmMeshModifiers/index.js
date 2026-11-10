// node.js
import * as fs from 'fs' //only if running in node.js
import rhino3dm from 'rhino3dm'

const rhino = await rhino3dm()

const buffer = fs.readFileSync('../../../models/AllMeshModifiers.3dm')
/*
if you are running this in a browser, you can replace the above line with these two lines:
const file = await fetch( '../../../models/decal.3dm' )
const buffer = await file.arrayBuffer()
*/
const arr = new Uint8Array(buffer)
const model = rhino.File3dm.fromByteArray(arr)
const objects = model.objects

console.log(`Number of Objects: ${objects.count}`)

const mm = [...objects][0].attributes.file3dmMeshModifiers

if( mm.displacement ) {

    console.log('---DISPLACEMENT---')
    console.log(`Displacement On: ${mm.displacement.on}`)
    console.log(`Displacement Texture: ${mm.displacement.textureId}`)
    console.log(model.renderTextures.find(mm.displacement.textureId).filename)
    console.log(`Displacement BlackPoint: ${mm.displacement.blackPoint}`)
    console.log(`Displacement WhitePoint: ${mm.displacement.whitePoint}`)
    console.log(`Displacement PostWeldAngle: ${mm.displacement.postWeldAngle}`)
    console.log(`Displacement FairingOn: ${mm.displacement.fairingOn}`)
    console.log(`Displacement Fairing: ${mm.displacement.fairing}`)
    console.log(`Displacement FinalMaxFaces: ${mm.displacement.finalMaxFaces}`)
    console.log(`Displacement FinalMaxFacesOn: ${mm.displacement.finalMaxFacesOn}`)
    console.log(`Displacement InitialQuality: ${mm.displacement.initialQuality}`)
    console.log(`Displacement MappingChannel: ${mm.displacement.mappingChannel}`)
    console.log(`Displacement MeshMemoryLimit: ${mm.displacement.meshMemoryLimit}`)
    console.log(`Displacement RefineSteps: ${mm.displacement.refineSteps}`)
    console.log(`Displacement RefineSensitivity: ${mm.displacement.refineSensitivity}`)
    console.log(`Displacement SweepResolutionFormula: ${mm.displacement.sweepResolutionFormula.constructor.name }`)
    console.log()

    mm.displacement.on = false
    mm.displacement.textureId = [...model.renderTextures][1].id
    mm.displacement.blackPoint = 0.12
    mm.displacement.whitePoint = 0.98
    mm.displacement.postWeldAngle = 25.0
    mm.displacement.fairingOn = true
    mm.displacement.fairing = 3
    mm.displacement.finalMaxFaces = 5000
    mm.displacement.finalMaxFacesOn = true
    mm.displacement.initialQuality = 500
    mm.displacement.mappingChannel = 2
    mm.displacement.meshMemoryLimit = 1024
    mm.displacement.refineSteps = 2
    mm.displacement.refineSensitivity = 0.75
    mm.displacement.sweepResolutionFormula = rhino.File3dmDisplacementSweepResolutionFormulas.AbsoluteToleranceDependent

    console.log('---DISPLACEMENT UPDATE---')
    console.log(`Displacement On: ${mm.displacement.on}`)
    console.log(`Displacement Texture: ${mm.displacement.textureId}`)
    console.log(model.renderTextures.find(mm.displacement.textureId).filename)
    console.log(`Displacement BlackPoint: ${mm.displacement.blackPoint}`)
    console.log(`Displacement WhitePoint: ${mm.displacement.whitePoint}`)
    console.log(`Displacement PostWeldAngle: ${mm.displacement.postWeldAngle}`)
    console.log(`Displacement FairingOn: ${mm.displacement.fairingOn}`)
    console.log(`Displacement Fairing: ${mm.displacement.fairing}`)
    console.log(`Displacement FinalMaxFaces: ${mm.displacement.finalMaxFaces}`)
    console.log(`Displacement FinalMaxFacesOn: ${mm.displacement.finalMaxFacesOn}`)
    console.log(`Displacement InitialQuality: ${mm.displacement.initialQuality}`)
    console.log(`Displacement MappingChannel: ${mm.displacement.mappingChannel}`)
    console.log(`Displacement MeshMemoryLimit: ${mm.displacement.meshMemoryLimit}`)
    console.log(`Displacement RefineSteps: ${mm.displacement.refineSteps}`)
    console.log(`Displacement RefineSensitivity: ${mm.displacement.refineSensitivity}`)
    console.log(`Displacement SweepResolutionFormula: ${mm.displacement.sweepResolutionFormula.constructor.name }`)
    console.log()

}

if ( mm.thickening ) {

    console.log('---EDGE SOFTENING---')
    console.log(`EdgeSoftening On: ${mm.edgeSoftening.on}`)
    console.log(`EdgeSoftening Softening: ${mm.edgeSoftening.softening}`)
    console.log(`EdgeSoftening Chamfer: ${mm.edgeSoftening.chamfer}`)
    console.log(`EdgeSoftening Faceted: ${mm.edgeSoftening.faceted}`)
    console.log(`EdgeSoftening EdgeAngleThreshold: ${mm.edgeSoftening.edgeAngleThreshold}`)
    console.log(`EdgeSoftening ForceSoftening: ${mm.edgeSoftening.forceSoftening}`)
    console.log()

    mm.edgeSoftening.on = false
    mm.edgeSoftening.softening = 2.345
    mm.edgeSoftening.chamfer = true
    mm.edgeSoftening.faceted = true
    mm.edgeSoftening.edgeAngleThreshold = 3.14
    mm.edgeSoftening.forceSoftening = true

    console.log('---EDGE SOFTENING UPDATE---')
    console.log(`EdgeSoftening On: ${mm.edgeSoftening.on}`)
    console.log(`EdgeSoftening Softening: ${mm.edgeSoftening.softening}`)
    console.log(`EdgeSoftening Chamfer: ${mm.edgeSoftening.chamfer}`)
    console.log(`EdgeSoftening Faceted: ${mm.edgeSoftening.faceted}`)
    console.log(`EdgeSoftening EdgeAngleThreshold: ${mm.edgeSoftening.edgeAngleThreshold}`)
    console.log(`EdgeSoftening ForceSoftening: ${mm.edgeSoftening.forceSoftening}`)
    console.log()

}

if( mm.thickening ) {

    console.log('---THICKENING---')
    console.log(`Thickening On: ${ mm.thickening.on }`)
    console.log(`Thickening distance: ${ mm.thickening.distance }`)
    console.log(`Thickening solid: ${ mm.thickening.solid }`)
    console.log(`Thickening offsetOnly: ${ mm.thickening.offsetOnly }`)
    console.log(`Thickening bothSides: ${ mm.thickening.bothSides }`)
    console.log()

    mm.thickening.on = false
    mm.thickening.distance = 2.34
    mm.thickening.solid = false
    mm.thickening.offsetOnly = true
    mm.thickening.bothSides = true

    console.log('---THICKENING UPDATE---')
    console.log(`Thickening On: ${ mm.thickening.on }`)
    console.log(`Thickening distance: ${ mm.thickening.distance }`)
    console.log(`Thickening solid: ${ mm.thickening.solid }`)
    console.log(`Thickening offsetOnly: ${ mm.thickening.offsetOnly }`)
    console.log(`Thickening bothSides: ${ mm.thickening.bothSides }`)
    console.log()

}

if( mm.curvePiping ) {

    console.log('---CURVE PIPING---')
    console.log(`Curve Piping On: ${ mm.curvePiping.on }`)
    console.log(`Curve Piping radius: ${ mm.curvePiping.radius }`)
    console.log(`Curve Piping segments: ${ mm.curvePiping.segments }`)
    console.log(`Curve Piping faceted: ${ mm.curvePiping.faceted }`)
    console.log(`Curve Piping accuracy: ${ mm.curvePiping.accuracy }`)
    console.log(`Curve Piping capType: ${ mm.curvePiping.capType.constructor.name }`)
    //console.log(`Curve Piping deleteAllCurves: ${ curvePiping.deleteAllCurves }`)
    console.log()

    mm.curvePiping.on = false
    mm.curvePiping.radius = 12.24
    mm.curvePiping.segments = 64
    mm.curvePiping.faceted = true
    mm.curvePiping.accuracy = 75
    mm.curvePiping.capType = rhino.File3dmCurvePipingCapTypes.Dome

    console.log('---CURVE PIPING UPDATE---')
    console.log(`Curve Piping On: ${ mm.curvePiping.on }`)
    console.log(`Curve Piping radius: ${ mm.curvePiping.radius }`)
    console.log(`Curve Piping segments: ${ mm.curvePiping.segments }`)
    console.log(`Curve Piping faceted: ${ mm.curvePiping.faceted }`)
    console.log(`Curve Piping accuracy: ${ mm.curvePiping.accuracy }`)
    console.log(`Curve Piping capType: ${ mm.curvePiping.capType.constructor.name }`)
    //console.log(`Curve Piping deleteAllCurves: ${ curvePiping.deleteAllCurves }`)
    console.log()

}

if ( mm.shutLining ) {
    console.log('---SHUT LINING---')
    console.log(`ShutLining On: ${mm.shutLining.on}`)
    console.log(`ShutLining Faceted: ${mm.shutLining.faceted}`)
    console.log(`ShutLining AutoUpdate: ${mm.shutLining.autoUpdate}`)
    console.log(`ShutLining ForceUpdate: ${mm.shutLining.forceUpdate}`)
    console.log(`ShutLining Curves Count: ${mm.shutLining.getCurves().length}`)
    console.log()

    console.log('---SHUT LINING CURVE---')
    // shut lining curves are addressed by their id
    const slc = mm.shutLining.getCurves()[0]
    console.log(`ShutLiningCurve Id: ${slc}`)
    console.log(`ShutLiningCurve Radius: ${mm.shutLining.curveRadius(slc)}`)
    console.log(`ShutLiningCurve Profile: ${mm.shutLining.curveProfile(slc)}`)
    console.log(`ShutLiningCurve Enabled: ${mm.shutLining.curveEnabled(slc)}`)
    console.log(`ShutLiningCurve Pull: ${mm.shutLining.curvePull(slc)}`)
    console.log(`ShutLiningCurve IsBump: ${mm.shutLining.curveIsBump(slc)}`)
    console.log()

    mm.shutLining.setCurveRadius(slc, 2.5)
    mm.shutLining.setCurveProfile(slc, 1)
    mm.shutLining.setCurveEnabled(slc, false)
    mm.shutLining.setCurvePull(slc, false)
    mm.shutLining.setCurveIsBump(slc, true)

    console.log('---SHUT LINING CURVE UPDATE---')
    console.log(`ShutLiningCurve Id: ${slc}`)
    console.log(`ShutLiningCurve Radius: ${mm.shutLining.curveRadius(slc)}`)
    console.log(`ShutLiningCurve Profile: ${mm.shutLining.curveProfile(slc)}`)
    console.log(`ShutLiningCurve Enabled: ${mm.shutLining.curveEnabled(slc)}`)
    console.log(`ShutLiningCurve Pull: ${mm.shutLining.curvePull(slc)}`)
    console.log(`ShutLiningCurve IsBump: ${mm.shutLining.curveIsBump(slc)}`)
    console.log()

    mm.shutLining.on = false
    mm.shutLining.faceted = true
    mm.shutLining.autoUpdate = true
    mm.shutLining.forceUpdate = true
    mm.shutLining.deleteAllCurves()

    console.log('---SHUT LINING UPDATE---')
    console.log(`ShutLining On: ${mm.shutLining.on}`)
    console.log(`ShutLining Faceted: ${mm.shutLining.faceted}`)
    console.log(`ShutLining AutoUpdate: ${mm.shutLining.autoUpdate}`)
    console.log(`ShutLining ForceUpdate: ${mm.shutLining.forceUpdate}`)
    console.log(`ShutLining Curves Count: ${mm.shutLining.getCurves().length}`)
    console.log()

}