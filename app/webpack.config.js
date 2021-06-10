const HtmlWebpackPlugin = require('html-webpack-plugin')
const {CleanWebpackPlugin} = require('clean-webpack-plugin')
const ESLintPlugin = require('eslint-webpack-plugin')
const path = require('path')

const OUTPUT_FOLDER = path.join(__dirname, 'dist')
const INDEX_HTML = path.join(__dirname, '/index.html')

module.exports = {
    entry: './src/index.tsx',
    output: {
        path: OUTPUT_FOLDER,
        filename: 'bundle.js',
    },
    module: {
        rules: [
            {
                test: /\.tsx?$/,
                use: 'babel-loader',
                exclude: /node_modules/
            },
            {
                test: /\.scss$/,
                use: [ 'style-loader', 'css-loader', 'sass-loader']
            },
            {
                test: /\.css$/,
                use: [ 'style-loader', 'css-loader']
            },
        ],
    },
    resolve: {
        extensions: ['.js', '.jsx', '.ts', '.tsx']
    },
    plugins: [
        new HtmlWebpackPlugin({template: INDEX_HTML}),
        new CleanWebpackPlugin(),
        new ESLintPlugin({extensions: ['js', 'jsx', 'ts', 'tsx']}),
    ],
    devServer: {
        proxy: {
            '/api': {
                target: 'http://192.168.178.38',
                pathRewrite: {'^/api': ''},
            },
        },
    },
    devtool: 'source-map',
}
