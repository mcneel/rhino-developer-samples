const { merge } = require('webpack-merge');
const common = require('./webpack.common');

//Configure dev enviroment by combining common configuration and adding some more options
module.exports = merge(common, {
    mode: 'development',
    devtool: 'inline-source-map',
    devServer: {
        static: './dist',
        open: true,
        hot: true
    }
})