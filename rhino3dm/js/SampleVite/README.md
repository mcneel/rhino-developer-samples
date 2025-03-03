#rhino3dm.js + vite

This is a quick sample using rhino3dm.js in Vite.

The app was created with `npm create vite@latest`

To run: `npm run dev`

Note: There are three things I needed to do to make this work:

1. Import rhino3dm: `npm i rhino3dm`
2. Change the 'main' entry file to rhino3dm to use the module. This change is in `node_modules/rhino3dm/package.json`:  "main": "rhino3dm.module.js"
3. Create a vite.config.js file in the root of the project with the following contents:

```js
import {defineConfig} from 'vite'
export default defineConfig({
  optimizeDeps: {
    exclude: ['rhino3dm'],
  },
});
```