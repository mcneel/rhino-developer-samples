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
const objects = model.objects()

console.log(`Number of Objects: ${objects.count}`)

const mm = objects.get( 0 ).attributes().meshModifiers()

if( mm.displacement() ) {

    console.log('---DISPLACEMENT---')
    console.log(`Displacement On: ${mm.displacement().on}`)
    console.log(`Displacement Texture: ${mm.displacement().texture}`)
    console.log(model.renderContent().findId(mm.displacement().texture).toTexture().fileName)
    console.log(`Displacement BlackPoint: ${mm.displacement().blackPoint}`)
    console.log(`Displacement WhitePoint: ${mm.displacement().whitePoint}`)
    console.log(`Displacement PostWeldAngle: ${mm.displacement().postWeldAngle}`)
    console.log(`Displacement FairingOn: ${mm.displacement().fairingOn}`)
    console.log(`Displacement Fairing: ${mm.displacement().fairing}`)
    console.log(`Displacement FinalMaxFaces: ${mm.displacement().finalMaxFaces}`)
    console.log(`Displacement FinalMaxFacesOn: ${mm.displacement().finalMaxFacesOn}`)
    console.log(`Displacement InitialQuality: ${mm.displacement().initialQuality}`)
    console.log(`Displacement MappingChannel: ${mm.displacement().mappingChannel}`)
    console.log(`Displacement MeshMemoryLimit: ${mm.displacement().meshMemoryLimit}`)
    console.log(`Displacement RefineSteps: ${mm.displacement().refineSteps}`)
    console.log(`Displacement RefineSensitivity: ${mm.displacement().refineSensitivity}`)
    console.log(`Displacement SweepResolutionFormula: ${mm.displacement().sweepResolutionFormula.constructor.name }`)
    console.log()

    mm.displacement().on = false
    mm.displacement().texture = model.renderContent().get(2).id
    mm.displacement().blackPoint = 0.12
    mm.displacement().whitePoint = 0.98
    mm.displacement().postWeldAngle = 25.0
    mm.displacement().fairingOn = true
    mm.displacement().fairing = 3
    mm.displacement().finalMaxFaces = 5000
    mm.displacement().finalMaxFacesOn = true
    mm.displacement().initialQuality = 500
    mm.displacement().mappingChannel = 2
    mm.displacement().meshMemoryLimit = 1024
    mm.displacement().refineSteps = 2
    mm.displacement().refineSensitivity = 0.75
    mm.displacement().sweepResolutionFormula = rhino.DisplacementSweepResolutionFormulas.AbsoluteToleranceDependent

    console.log('---DISPLACEMENT UPDATE---')
    console.log(`Displacement On: ${mm.displacement().on}`)
    console.log(`Displacement Texture: ${mm.displacement().texture}`)
    console.log(model.renderContent().findId(mm.displacement().texture).toTexture().fileName)
    console.log(`Displacement BlackPoint: ${mm.displacement().blackPoint}`)
    console.log(`Displacement WhitePoint: ${mm.displacement().whitePoint}`)
    console.log(`Displacement PostWeldAngle: ${mm.displacement().postWeldAngle}`)
    console.log(`Displacement FairingOn: ${mm.displacement().fairingOn}`)
    console.log(`Displacement Fairing: ${mm.displacement().fairing}`)
    console.log(`Displacement FinalMaxFaces: ${mm.displacement().finalMaxFaces}`)
    console.log(`Displacement FinalMaxFacesOn: ${mm.displacement().finalMaxFacesOn}`)
    console.log(`Displacement InitialQuality: ${mm.displacement().initialQuality}`)
    console.log(`Displacement MappingChannel: ${mm.displacement().mappingChannel}`)
    console.log(`Displacement MeshMemoryLimit: ${mm.displacement().meshMemoryLimit}`)
    console.log(`Displacement RefineSteps: ${mm.displacement().refineSteps}`)
    console.log(`Displacement RefineSensitivity: ${mm.displacement().refineSensitivity}`)
    console.log(`Displacement SweepResolutionFormula: ${mm.displacement().sweepResolutionFormula.constructor.name }`)
    console.log()

}

if ( mm.thickening() ) {

    console.log('---EDGE SOFTENING---')
    console.log(`EdgeSoftening On: ${mm.edgeSoftening().on}`)
    console.log(`EdgeSoftening Softening: ${mm.edgeSoftening().softening}`)
    console.log(`EdgeSoftening Chamfer: ${mm.edgeSoftening().chamfer}`)
    console.log(`EdgeSoftening Faceted: ${mm.edgeSoftening().faceted}`)
    console.log(`EdgeSoftening EdgeAngleThreshold: ${mm.edgeSoftening().edgeAngleThreshold}`)
    console.log(`EdgeSoftening ForceSoftening: ${mm.edgeSoftening().forceSoftening}`)
    console.log()

    mm.edgeSoftening().on = false
    mm.edgeSoftening().softening = 2.345
    mm.edgeSoftening().chamfer = true
    mm.edgeSoftening().faceted = true
    mm.edgeSoftening().edgeAngleThreshold = 3.14
    mm.edgeSoftening().forceSoftening = true

    console.log('---EDGE SOFTENING UPDATE---')
    console.log(`EdgeSoftening On: ${mm.edgeSoftening().on}`)
    console.log(`EdgeSoftening Softening: ${mm.edgeSoftening().softening}`)
    console.log(`EdgeSoftening Chamfer: ${mm.edgeSoftening().chamfer}`)
    console.log(`EdgeSoftening Faceted: ${mm.edgeSoftening().faceted}`)
    console.log(`EdgeSoftening EdgeAngleThreshold: ${mm.edgeSoftening().edgeAngleThreshold}`)
    console.log(`EdgeSoftening ForceSoftening: ${mm.edgeSoftening().forceSoftening}`)
    console.log()

}

if( mm.thickening() ) {

    console.log('---THICKENING---')
    console.log(`Thickening On: ${ mm.thickening().on }`)
    console.log(`Thickening distance: ${ mm.thickening().distance }`)
    console.log(`Thickening solid: ${ mm.thickening().solid }`)
    console.log(`Thickening offsetOnly: ${ mm.thickening().offsetOnly }`)
    console.log(`Thickening bothSides: ${ mm.thickening().bothSides }`)
    console.log()

    mm.thickening().on = false
    mm.thickening().distance = 2.34
    mm.thickening().solid = false
    mm.thickening().offsetOnly = true
    mm.thickening().bothSides = true

    console.log('---THICKENING UPDATE---')
    console.log(`Thickening On: ${ mm.thickening().on }`)
    console.log(`Thickening distance: ${ mm.thickening().distance }`)
    console.log(`Thickening solid: ${ mm.thickening().solid }`)
    console.log(`Thickening offsetOnly: ${ mm.thickening().offsetOnly }`)
    console.log(`Thickening bothSides: ${ mm.thickening().bothSides }`)
    console.log()

}

if( mm.curvePiping() ) {

    console.log('---CURVE PIPING---')
    console.log(`Curve Piping On: ${ mm.curvePiping().on }`)
    console.log(`Curve Piping radius: ${ mm.curvePiping().radius }`)
    console.log(`Curve Piping segments: ${ mm.curvePiping().segments }`)
    console.log(`Curve Piping faceted: ${ mm.curvePiping().faceted }`)
    console.log(`Curve Piping accuracy: ${ mm.curvePiping().accuracy }`)
    console.log(`Curve Piping capType: ${ mm.curvePiping().capType.constructor.name }`)
    //console.log(`Curve Piping deleteAllCurves: ${ curvePiping.deleteAllCurves }`)
    console.log()

    mm.curvePiping().on = false
    mm.curvePiping().radius = 12.24
    mm.curvePiping().segments = 64
    mm.curvePiping().faceted = true
    mm.curvePiping().accuracy = 75
    mm.curvePiping().capType = rhino.CurvePipingCapTypes.Dome

    console.log('---CURVE PIPING UPDATE---')
    console.log(`Curve Piping On: ${ mm.curvePiping().on }`)
    console.log(`Curve Piping radius: ${ mm.curvePiping().radius }`)
    console.log(`Curve Piping segments: ${ mm.curvePiping().segments }`)
    console.log(`Curve Piping faceted: ${ mm.curvePiping().faceted }`)
    console.log(`Curve Piping accuracy: ${ mm.curvePiping().accuracy }`)
    console.log(`Curve Piping capType: ${ mm.curvePiping().capType.constructor.name }`)
    //console.log(`Curve Piping deleteAllCurves: ${ curvePiping.deleteAllCurves }`)
    console.log()

}

if ( mm.shutLining() ) {
    console.log('---SHUT LINING---')
    console.log(`ShutLining On: ${mm.shutLining().on}`)
    console.log(`ShutLining Faceted: ${mm.shutLining().faceted}`)
    console.log(`ShutLining AutoUpdate: ${mm.shutLining().autoUpdate}`)
    console.log(`ShutLining ForceUpdate: ${mm.shutLining().forceUpdate}`)
    console.log(`ShutLining Curves Count: ${mm.shutLining().curves.count}`)
    console.log()

    console.log('---SHUT LINING CURVE---')
    const slc = mm.shutLining().curves.findIndex(0)
    console.log(`ShutLiningCurve Id: ${slc.id}`)
    console.log(`ShutLiningCurve Radius: ${slc.radius}`)
    console.log(`ShutLiningCurve Profile: ${slc.profile}`)
    console.log(`ShutLiningCurve Enabled: ${slc.enabled}`)
    console.log(`ShutLiningCurve Pull: ${slc.pull}`)
    console.log(`ShutLiningCurve IsBump: ${slc.isBump}`)
    console.log()

    slc.id = '2fed634d-0000-0000-0000-cfd10ef3ad6d'
    slc.radius = 2.5
    slc.profile = 1
    slc.enabled = false
    slc.pull = false
    slc.isBump = true

    console.log('---SHUT LINING CURVE UPDATE---')
    console.log(`ShutLiningCurve Id: ${slc.id}`)
    console.log(`ShutLiningCurve Radius: ${slc.radius}`)
    console.log(`ShutLiningCurve Profile: ${slc.profile}`)
    console.log(`ShutLiningCurve Enabled: ${slc.enabled}`)
    console.log(`ShutLiningCurve Pull: ${slc.pull}`)
    console.log(`ShutLiningCurve IsBump: ${slc.isBump}`)
    console.log()

    mm.shutLining().on = false
    mm.shutLining().faceted = true
    mm.shutLining().autoUpdate = true
    mm.shutLining().forceUpdate = true
    mm.shutLining().deleteAllCurves()

    console.log('---SHUT LINING UPDATE---')
    console.log(`ShutLining On: ${mm.shutLining().on}`)
    console.log(`ShutLining Faceted: ${mm.shutLining().faceted}`)
    console.log(`ShutLining AutoUpdate: ${mm.shutLining().autoUpdate}`)
    console.log(`ShutLining ForceUpdate: ${mm.shutLining().forceUpdate}`)
    console.log(`ShutLining Curves Count: ${mm.shutLining().curves.count}`)
    console.log()

}