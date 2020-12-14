const rhino3dm = require('rhino3dm')
const compute = require('compute-rhino3d')

rhino3dm().then(async rhino => {

    console.log('rhino3dm loaded.')

    compute.url = "http://localhost:8081/" // Rhino.Compute server url
    compute.apiKey = "" // your Rhino.Compute server api key

    const crvPoints = new rhino.Point3dList()
    crvPoints.add( 0, 0, 0,)
    crvPoints.add( 10, 10, 0,)
    crvPoints.add( 20, -10, 0,)
    crvPoints.add( 30, 10, 20,)
    crvPoints.add( 40, -10, -20,)
    crvPoints.add( 50, 0, 0,)

    const nCrv = rhino.NurbsCurve.create( false, 3, crvPoints)

    const result = await compute.Curve.fit( nCrv, 2, 1.0, 1.0)

    console.log(result)

})