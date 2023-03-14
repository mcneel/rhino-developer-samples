# Convert OBJ to 3DM with threejs and rhino3dm

An example cli tool that reads an obj file and saves it as a 3dm

```commandline
$ npm install
$ node index.js
```
## note 

If using node versions 18.1.0 and higher, and rhino3dm.js 7.15 and lower, you need to add the `--no-experimental-fetch` flag to the command:

```commandline
$ node index.js
```
 see this issue for details: https://github.com/mcneel/rhino3dm/issues/492

 This issue has been fixed by updating the build tools that make rhino3dm.js.