# HelloRhinoCompute

This sample is a little different to the others; it's a plug-in for Rhino that exposes a couple of custom endpoints in Compute.

* _/sample/convert3dmtopdf-string_ - takes a base64-encoded 3dm file and returns a base64-encoded pdf
* _/sample/convert3dmtodwg-string_guid_ - takes a base64-encoded dwg and a layer ID and returns a base64-encoded dwg

The custom endpoint extention framework is currently built around JSON, hence all the base64-encoding!

Here's an example node.js client calling the new endpoints.

```js
const fs = require('fs')
const RhinoCompute = require('compute-rhino3d')

// if you have a different Rhino.Compute server, add the URL here:
RhinoCompute.url = 'http://localhost:8081/'
// if the above is not localhost, enter the api key here:
//RhinoCompute.apiKey = ''

const file3dmpath = './files/Rhino_Logo.3dm'

compute()

async function compute() {

    // example 1 - convert a 3dm file to pdf

    try {
        const buffer = fs.readFileSync(file3dmpath)
        const b64ba = buffer.toString('base64')
        const res = await RhinoCompute.computeFetch('sample/convert3dmtopdf-string', [b64ba], true)

        // do something with the response
        const buff = Buffer.from(res, 'base64')
        fs.writeFileSync('out.pdf', buff)

    } catch (error) {
        console.error(error)
    }

    // example 2 - convert a single layer (by id) of a 3d file to dwg

    try {
      const buffer = fs.readFileSync(file3dmpath)
      const b64ba = buffer.toString('base64')
      const res = await RhinoCompute.computeFetch('sample/convert3dmtodwg-string_guid', [b64ba, '2726aa36-732e-488e-9cf4-0b7cf82e65ce'], true)

      // do something with the response
      const buff = Buffer.from(res, 'base64')
      fs.writeFileSync('out.dwg', buff)
    
    } catch (error) {
      console.error(error)
    }

}
```
