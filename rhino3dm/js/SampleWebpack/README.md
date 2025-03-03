# rhino3dm + webpack

Sample that shows how to load rhino3dm.wasm module with webpack

To run: 
```
npm i
npm run dev
```

These are the steps that I took to create this sample:

1. follow this example without getting into vue: https://medium.com/@gersonjunior12/vue-js-3-webpack-5-minimal-configuration-35e535f399fd
2. import the copy-webpack-plugin: `npm i copy-webpack-plugin -D`
2. import rhino3dm: `npm i rhino3dm`
3. Change the 'main' entry file to rhino3dm to use the module. This change is in `node_modules/rhino3dm/package.json: "main": "rhino3dm.module.js"`
4. See `config/webpack.common.js` for additional changes to be able to load wasm