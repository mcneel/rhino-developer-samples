#rhino3dm.js + vite

This is a quick sample using rhino3dm.js in Vite.

The app was created with `npm create vite@latest`

To run: `npm run dev`

To make this work, I have the rhino3dm library imported in a script tag in the index.html. 
rhino3dm is referenced in counter.js and a sphere is created as the counter increments.

Maybe there is a better way to import the library, but other attempts to import the library via npm always resulted in errors.

If someone knows how to use npm to manage the rhino3dm dependency so that it can be imported, then please suggest the changes.