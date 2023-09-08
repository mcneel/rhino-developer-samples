import 'dotenv/config'
import RhinoCompute from 'compute-rhino3d'

RhinoCompute.url = process.env.RHINO_COMPUTE_URL
RhinoCompute.apiKey = process.env.RHINO_COMPUTE_KEY

const endpoint = 'healthcheck'

try {

    const res = await RhinoCompute.computeFetch(endpoint, null, false) //healthcheck does not return json
    const txt = await res.text()
    console.log(txt)

} catch (error) {

    console.error(error)

}

