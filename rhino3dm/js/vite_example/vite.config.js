import {defineConfig} from 'vite'
export default defineConfig({
  optimizeDeps: {
    exclude: ['rhino3dm'],
  },
});