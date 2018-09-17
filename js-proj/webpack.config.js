const HtmlWebPackPlugin = require('html-webpack-plugin');
const MiniCssExtractPlugin = require('mini-css-extract-plugin');
const path = require('path');

module.exports = (env, options) => {
  const isProduction = options.mode === 'production' ? true : false;

  return {
    entry: [path.join(__dirname, 'index.js')], output: {
      path: path.join(__dirname, 'static'),
      filename: `[name]${isProduction ? '.min' : ''}.js`,
      publicPath: '/static'
    },
        resolve: {extensions: ['.js', '.jsx']}, devServer: {
          contentBase: path.join(__dirname),
          port: 8080,
        },
        devtool: isProduction ? false : 'source-map', module: {
          rules:
              [
                {
                  test: /\.(js|jsx)$/,
                  exclude: /node_modules/,
                  use: {
                    loader: 'babel-loader',
                    options: {
                      presets: ['@babel/preset-env', '@babel/preset-react'],
                      plugins: ['@babel/plugin-transform-runtime']
                    },
                  }
                },
                {
                  test: /\.html$/,
                  use: [{
                    loader: 'html-loader',
                    options: {
                      minimize: true,
                    }
                  }]
                },
                {
                  test: /\.css$/,
                  use: [
                    MiniCssExtractPlugin.loader, {
                      loader: 'css-loader',
                      options:
                          {minimize: isProduction, sourceMap: !isProduction}
                    },
                    {
                      loader: 'postcss-loader',
                      options: {plugins: [require('autoprefixer')]}
                    }
                  ],
                }
              ]
        },
        plugins: [
          new HtmlWebPackPlugin(
              {template: './src/index.html', filename: './index.html'}),
          new MiniCssExtractPlugin(
              {filename: '[name].css', chunkFilename: '[id].css'})
        ]
  }
};