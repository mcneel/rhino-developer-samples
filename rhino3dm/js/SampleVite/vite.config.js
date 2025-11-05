import {defineConfig} from 'vite'
export default defineConfig({
  optimizeDeps: {
    exclude: ['rhino3dm'],
  },
  // use the rhino3dm.module.js entrypoint
  resolve: {
    alias: {
      'rhino3dm': path.resolve(__dirname, 'node_modules/rhino3dm/rhino3dm.module.js')
    }
  }
})