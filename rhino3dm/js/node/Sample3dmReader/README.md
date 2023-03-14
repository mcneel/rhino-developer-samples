# Simple 3dm file reader

An example cli tool that reads a 3dm file and prints some stuff about it!

```commandline
$ npm install
$ node index.js <path_to_file.3dm>
```

## note 

If using node versions 18.1.0 and higher, and rhino3dm.js 7.15 and lower, you need to add the `--no-experimental-fetch` flag to the command:

```commandline
$ node index.js <path_to_file.3dm>
```
 see this issue for details: https://github.com/mcneel/rhino3dm/issues/492

 This issue has been fixed by updating the build tools that make rhino3dm.js.