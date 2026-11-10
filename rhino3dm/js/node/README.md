# rhino3dm.js node.js samples

Each folder is a small node.js program that uses [rhino3dm.js](https://www.npmjs.com/package/rhino3dm). The folders share one install: run `npm install` once in this folder, then run a sample from here

```
npm install
npm start -w sample3dmreader
```

or from its own folder with `npm start` (or `node index.js`). `npm test` runs every sample.

The samples need node.js 24 or later. Each folder's `package.json` lists what that sample uses, so you can copy one folder out and run `npm install` in it.

| Sample | What it shows |
| --- | --- |
| [Sample3dmMeshModifiers](Sample3dmMeshModifiers) | Read the mesh modifiers on the objects in a model |
| [Sample3dmReader](Sample3dmReader) | Read a 3dm file |
| [SampleAddGroups](SampleAddGroups) | Add groups to a model and write it |
| [SampleArc](SampleArc) | Make an arc |
| [SampleArcCurve](SampleArcCurve) | Make an arc curve |
| [SampleDecodeObjects](SampleDecodeObjects) | Encode geometry to JSON and decode it again |
| [SampleExportOBJ](SampleExportOBJ) | Export the meshes in a model to OBJ with three.js |
| [SampleNurbsCurve](SampleNurbsCurve) | Read a NURBS curve's properties |
| [SampleNurbsSurface](SampleNurbsSurface) | Make a NURBS surface and write it |
| [SampleObjTo3dm](SampleObjTo3dm) | Read an OBJ with three.js and write it as a 3dm file |
| [SamplePointCloud](SamplePointCloud) | Make a point cloud |
| [SampleReadTextEntities](SampleReadTextEntities) | Read the text entities in a model |
| [SampleViewports](SampleViewports) | Read a model's views and named views |
| [Samples](Samples) | Add an instance definition, add a material with an embedded texture, read a model's materials |
