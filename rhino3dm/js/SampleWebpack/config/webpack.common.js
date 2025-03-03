const path = require('path');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const CopyWebpackPlugin = require('copy-webpack-plugin');

//Just to help us with directories and folders path
const __base = path.resolve(__dirname, '..');
const __src = path.resolve(__base, 'src');

module.exports = {
    //Entry: main file that init our application
    entry: path.resolve(__src, 'main.js'),

    // needed to run rhino3dm.wasm
    resolve: {
        fallback: {
            crypto: false,
            fs: false,
            path: false
        }},

    node: {
        global: false,
        __filename: false,             
        __dirname: false,
    },

    //Output: result of the bundle after webpack run
    output: {
        filename: '[name].bundle.js',
        path: path.resolve(__base, 'dist'),
        clean: true
    },
    module:{
        rules: [{
        test: /\.wasm$/,
        use: "file-loader"
    }]},

    //Plugins to help and include additionals functionalities to webpack
    plugins: [
        new HtmlWebpackPlugin({
            title: 'Minimal rhino3dm Webpack',
            //favicon: path.resolve(__src, 'static', 'favicon.ico'),
            template: path.resolve(__src, 'templates', 'index.html'),
        }),
        new CopyWebpackPlugin({
            patterns:
            [ { from: 'node_modules/rhino3dm/rhino3dm.wasm', to: './' } ]
        })
    ]
}