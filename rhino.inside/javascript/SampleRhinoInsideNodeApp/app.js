const edge = require('edge-js')
const rhino3dm = require('rhino3dm')

// construct the path to the .NET dll
const namespace = 'SampleRhinoInsideJavascriptMethodsLib'
const baseAppPath = '../' + namespace + '/bin/Debug/'
const baseDll = baseAppPath + namespace + '.dll'

// console.log(baseDll)

const rhinoTypeName = namespace + '.RhinoMethods'

const startRhino = edge.func({
    assemblyFile: baseDll,
    typeName: rhinoTypeName,
    methodName: 'StartRhino'
})

const makeSphere = edge.func({
    assemblyFile: baseDll,
    typeName: rhinoTypeName,
    methodName: 'MakeSphere'
})

rhino3dm().then((rhino) => {

  console.log('rhino3dm loaded.')

  startRhino('', function (error, result) {
    if (error) throw error
    console.log(result)

    // Try changing these parameters to get different results from the 'makeSphere' call.
    let params = {radius: 10.00, countU: 10, countV:10 }

    makeSphere(params, function (error, result) {
      if (error) throw error
      //convert this to object
      const obj = JSON.parse(result)
      const rhinoMesh = rhino.CommonObject.decode(obj)
      console.log(`Mesh has ${rhinoMesh.vertices().count} vertices and ${rhinoMesh.faces().count} faces.`)
      rhinoMesh.delete()
    })

  })

})


