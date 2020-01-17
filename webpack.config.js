const webpack = require('webpack');
const MiniCssExtractPlugin = require('mini-css-extract-plugin');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const path = require('path');
const outputDir = path.join(__dirname, 'dist/');

const isProd = process.env.NODE_ENV === 'production';

module.exports = {
	entry: './lib/js/src/Main.js',
	mode:
		isProd ? 'production' :
		'development',
	devtool: 'source-map',
	output: {
		path: outputDir,
		filename: 'index.js'
	},
	module: {
		rules: [
			{
				test: /\.less$/,
				use: [
					{
						loader: 'style-loader' // creates style nodes from JS strings
					},
					{
						loader: 'css-loader' // translates CSS into CommonJS
					},
					{
						loader: 'less-loader' // compiles Less to CSS
					}
				]
			},
			{
				test: /\.s[ac]ss$/i,
				use: [
					// Creates `style` nodes from JS strings
					'style-loader',
					// Translates CSS into CommonJS
					'css-loader',
					// Compiles Sass to CSS
					'sass-loader'
				]
			},
			{
				test: /\.css$/,
				use: [
					{
						loader: MiniCssExtractPlugin.loader,
						options: {
							publicPath: outputDir + '/'
						}
					},
					'css-loader'
				]
			},
			{
				test: /\.m?js$/,
				exclude: /(node_modules|bower_components)/,
				use: {
					loader: 'babel-loader',
					options: {
						presets: [
							'@babel/preset-env'
						]
					}
				}
			},
			{
				test: /\.(png|jpg)$/i,
				use: [
					{
						loader: 'url-loader',
						options: {
							limit: 10000
						}
					}
				]
			}
		]
	},
	plugins: [
		new MiniCssExtractPlugin({
			// Options similar to the same options in webpackOptions.output
			// both options are optional
			filename: '[name].css',
			chunkFilename: '[id].css'
		}),
		new HtmlWebpackPlugin({
			template: 'src/index.html'
		}),
		new webpack.DefinePlugin({
			PRODUCTION: isProd
		})
	],
	devServer: {
		compress: true,
		contentBase: outputDir,
		port: process.env.PORT || 8080,
		historyApiFallback: true
	}
};
