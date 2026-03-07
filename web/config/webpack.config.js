"use strict";

const path = require("path");
const fs = require("fs");
const webpack = require("webpack");
const HtmlWebpackPlugin = require("html-webpack-plugin");
const MiniCssExtractPlugin = require("mini-css-extract-plugin");
const CssMinimizerPlugin = require("css-minimizer-webpack-plugin");
const TerserPlugin = require("terser-webpack-plugin");
const { WebpackManifestPlugin } = require("webpack-manifest-plugin");
const WorkboxWebpackPlugin = require("workbox-webpack-plugin");
const ESLintPlugin = require("eslint-webpack-plugin");
const ReactRefreshWebpackPlugin = require("@pmmmwh/react-refresh-webpack-plugin");

const appDirectory = fs.realpathSync(process.cwd());
const resolveApp = (relativePath) => path.resolve(appDirectory, relativePath);

const isEnvProduction = process.env.NODE_ENV === "production";
const isEnvDevelopment = !isEnvProduction;

module.exports = {
  mode: isEnvProduction ? "production" : "development",
  devtool: isEnvProduction ? "source-map" : "cheap-module-source-map",
  entry: resolveApp("src/index.js"),
  output: {
    path: resolveApp("build"),
    // 修复：开发环境使用不同的文件名格式避免冲突
    filename: isEnvProduction
      ? "static/js/[name].[contenthash:8].js"
      : "static/js/[name].bundle.js",
    chunkFilename: isEnvProduction
      ? "static/js/[name].[contenthash:8].chunk.js"
      : "static/js/[name].chunk.js",
    publicPath: "/",
    clean: true,
  },
  resolve: {
    extensions: [".js", ".jsx", ".ts", ".tsx", ".json"],
    alias: {
      "@": resolveApp("src"),
    },
    fallback: {
      crypto: require.resolve("crypto-browserify"),
      stream: require.resolve("stream-browserify"),
      buffer: require.resolve("buffer"),
    },
  },
  module: {
    rules: [
      {
        test: /\.(js|jsx|ts|tsx)$/,
        include: resolveApp("src"),
        use: [
          {
            loader: "babel-loader",
            options: {
              cacheDirectory: true,
              plugins: [
                isEnvDevelopment && require.resolve("react-refresh/babel"),
              ].filter(Boolean),
            },
          },
        ],
      },
      {
        test: /\.css$/i,
        use: [
          isEnvProduction ? MiniCssExtractPlugin.loader : "style-loader",
          "css-loader",
          "postcss-loader",
        ],
      },
      {
        test: /\.(scss|sass)$/i,
        use: [
          isEnvProduction ? MiniCssExtractPlugin.loader : "style-loader",
          "css-loader",
          "sass-loader",
        ],
      },
      {
        test: /\.less$/i,
        use: [
          isEnvProduction ? MiniCssExtractPlugin.loader : "style-loader",
          "css-loader",
          "less-loader",
        ],
      },
      {
        test: /\.(png|jpe?g|gif|bmp|svg|webp)$/i,
        type: "asset",
        parser: { dataUrlCondition: { maxSize: 10 * 1024 } },
        generator: { filename: "static/media/[name].[contenthash:8][ext]" },
      },
      {
        test: /\.(woff2?|eot|ttf|otf)$/i,
        type: "asset/resource",
        generator: { filename: "static/fonts/[name].[contenthash:8][ext]" },
      },
      {
        test: /\.(docx?|pdf)$/i,
        type: "asset/resource",
        generator: { filename: "static/docs/[name].[contenthash:8][ext]" },
      },
    ],
  },
  plugins: [
    new HtmlWebpackPlugin({
      inject: true,
      template: resolveApp("public/index.html"),
      minify: isEnvProduction,
    }),
    isEnvDevelopment && new ReactRefreshWebpackPlugin(),
    isEnvProduction &&
      new MiniCssExtractPlugin({
        filename: "static/css/[name].[contenthash:8].css",
        chunkFilename: "static/css/[name].[contenthash:8].chunk.css",
      }),
    new ESLintPlugin({
      extensions: ["js", "jsx", "ts", "tsx"],
      failOnError: isEnvProduction,
    }),
    isEnvProduction &&
      new WebpackManifestPlugin({
        fileName: "asset-manifest.json",
        publicPath: "/",
      }),
    isEnvProduction &&
      new WorkboxWebpackPlugin.GenerateSW({
        clientsClaim: true,
        skipWaiting: true,
      }),
    new webpack.IgnorePlugin({
      resourceRegExp: /^\.\/locale$/,
      contextRegExp: /moment$/,
    }),
    new webpack.DefinePlugin({
      "process.env.NODE_ENV": JSON.stringify(process.env.NODE_ENV),
      "process.env.BABEL_ENV": JSON.stringify(process.env.BABEL_ENV),
    }),
    new webpack.ProvidePlugin({
      Buffer: ["buffer", "Buffer"],
    }),
  ].filter(Boolean),
  optimization: {
    minimize: isEnvProduction,
    minimizer: [
      new TerserPlugin({ extractComments: false }),
      new CssMinimizerPlugin(),
    ],
    // 修复：开发环境禁用代码分割，生产环境启用
    splitChunks: isEnvProduction
      ? {
          chunks: "all",
          name: false,
          cacheGroups: {
            vendor: {
              test: /[\\/]node_modules[\\/]/,
              name: "vendors",
              chunks: "all",
            },
          },
        }
      : false,
    // 修复：开发环境禁用 runtimeChunk
    runtimeChunk: isEnvProduction ? "single" : false,
  },
  devServer: {
    hot: true,
    historyApiFallback: {
      disableDotRule: true,
    },
    port: 3000,
    open: false,
    compress: true,
    client: {
      overlay: {
        errors: true,
        warnings: false,
      },
    },
  },
};
