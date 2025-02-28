# rhino3dm.js

[![](https://data.jsdelivr.com/v1/package/npm/rhino3dm/badge)](https://www.jsdelivr.com/package/npm/rhino3dm)

**rhino3dm.js** is a javascript library with associated web assembly (rhino3dm.wasm) that is OpenNURBS plus additional C++ to javascript bindings compiled to web assembly. The library based on OpenNURBS with a RhinoCommon style. The libraries will run on on all major browsers as well as node.js.

For browser based applications, we also develop a 3dmLoader for the popular three.js WebGL library: https://threejs.org/examples/?q=3dm#webgl_loader_3dm


## Usage

The easiest way to get started is to reference a specific version of the library using jsDelivr.

```html
<!DOCTYPE html>

<body>

  <!-- Import maps polyfill -->
  <!-- Remove this when import maps will be widely supported -->
  <script async src="https://unpkg.com/es-module-shims@1.10.0/dist/es-module-shims.js"></script>

  <script type="importmap">
      {
          "imports": {
            "rhino3dm":"https://cdn.jsdelivr.net/npm/rhino3dm@8.15.0-beta/rhino3dm.module.min.js"
          }
      }
  </script>

  <script type="module">

    import rhino3dm from 'rhino3dm'
    const rhino = await rhino3dm()
    const sphere = new rhino.Sphere( [1,2,3,], 12 )
    console.log(sphere.diameter)

  </script>

</body>

</html>
```

You can also [download the files](https://www.jsdelivr.com/package/npm/rhino3dm) if you want to bake them into your site or application. You'll need the `.wasm` web assembly along with the `.js` (or `.min.js`) wrapper.


### Node.js

**rhino3dm.js** is available on [npm](https://www.npmjs.com/package/rhino3dm); try `npm install rhino3dm`. Note: the resulting webassembly binary (rhino3dm.wasm) is fairly large for browser based applications. We are working on ways to make this smaller.

Example node.js script:

```js
import rhino3dm from 'rhino3dm'
const rhino = await rhino3dm()
const sphere = new rhino.Sphere([1,2,3,], 12)
console.log(sphere.diameter)
```

### React.js

Similar to plain html, **rhino3dm.js** can be added as a script in `index.js`

```js
// index.js
import { StrictMode } from "react";
import ReactDOM from "react-dom";

import App from "./App";

const rootElement = document.getElementById("root");

const script = document.createElement("script");
script.src = "https://cdn.jsdelivr.net/npm/rhino3dm@8.15.0-beta/rhino3dm.min.js";
script.addEventListener("load", () => {
  ReactDOM.render(
    <StrictMode>
      <App />
    </StrictMode>,
    rootElement
  );
});
document.body.appendChild(script);

```

Once loaded, `rhino3dm` can be referenced in any component with `window.rhino3dm`, keep in mind that the wasm file loads asycnhronously so any any calls to wasm methods need to happen inside `.then()` or using `await`

```js
// App.js
import React, { useEffect, useState } from "react";
import "./styles.css";

export default function App() {
  const [sphere, setSphere] = useState(null);

  useEffect(() => {
    window.rhino3dm().then((Module) => {
      //creating a sphere using rhino3dm
      setSphere(new Module.Sphere([1, 2, 3], 16));
    });
  }, []);

  return <div className="App">{sphere && <p>{`sphere diameter is: ${sphere.diameter}`}</p>}</div>;
}
```

CodeSandbox of above react implementation can be found [here](https://codesandbox.io/s/rhino3dm-react-p3gr7?file=/src/App.js:0-428)


## API Docs

The latest [rhino3dm.js API Documentation](https://mcneel.github.io/rhino3dm/javascript/api/index.html)


## Examples

There a few samples are available in the [Rhino Developer Samples repository](https://github.com/mcneel/rhino-developer-samples/tree/8/rhino3dm#samples)

An advanced sample creates a 3dm file viewer in a web browser.  The html+javascript to create the viewer is around 300 lines (including comments) and runs on all browsers including mobile devices.  

<img src="https://mcneel.github.io/rhino3dm/images/rhino3dm_rhinologo.png" width="300"></img>

**rhino3dm.js** is used to read a 3dm file and create an instance of a File3dm class in the browser’s memory.  It then walks through the objects in the model and calls compute.rhino3d.com to create meshes and isocurves for the polysurface. These meshes and isocurves are then added to a three.js scene for display.

Here's [another example](https://observablehq.com/@pearswj/using-rhino3dm-in-observable/2) of rhino3dm.js, this time running in one of [Observable](http://observablehq.com/)'s live notebooks. Dive right in an tweak the code!

## Build from source

If the pre-compiled libraries above do not work in your situation, you can compile the libraries from their source. For detailed instructions go to [rhino3dm.js and rhino3dm.wasm](RHINO3DM-BUILD.JS.md)
