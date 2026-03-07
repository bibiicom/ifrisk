'use strict';

process.env.BABEL_ENV = 'production';
process.env.NODE_ENV = 'production';

process.on('unhandledRejection', err => {
  throw err;
});

const path = require('path');
const fs = require('fs-extra');
const webpack = require('webpack');
const zlib = require('zlib');
const { promisify } = require('util');
const gzip = promisify(zlib.gzip);
const chalk = require('chalk');

const paths = require('../config/paths');
const configFactory = require('../config/webpack.config');

// 支持 config 工厂函数或直接对象
const config = typeof configFactory === 'function'
  ? configFactory('production')
  : configFactory;

const WARN_AFTER_BUNDLE_GZIP_SIZE = 512 * 1024;
const WARN_AFTER_CHUNK_GZIP_SIZE = 1024 * 1024;

async function measureFileSizes(buildDir) {
  if (!fs.existsSync(buildDir)) return {};
  const files = await fs.readdir(buildDir);
  const sizes = {};
  for (const file of files) {
    const filePath = path.join(buildDir, file);
    if (fs.statSync(filePath).isFile()) {
      const content = await fs.readFile(filePath);
      sizes[file] = (await gzip(content)).length;
    }
  }
  return sizes;
}

function printFileSizesAfterBuild(stats, previousSizes) {
  const outputPath = stats.compilation.outputOptions.path;
  const files = fs.readdirSync(outputPath);
  console.log(chalk.cyan('\nFile sizes after gzip:\n'));
  files.forEach(file => {
    const filePath = path.join(outputPath, file);
    if (fs.statSync(filePath).isFile()) {
      const content = fs.readFileSync(filePath);
      const size = zlib.gzipSync(content).length;
      const prevSize = previousSizes[file] || 0;
      const diff = size - prevSize;
      const diffStr = diff >= 0 ? `+${diff}` : `${diff}`;
      console.log(`${file}: ${size} bytes (gzip) [${diffStr}]`);
      if (size > WARN_AFTER_BUNDLE_GZIP_SIZE) {
        console.log(chalk.yellow(`Warning: ${file} exceeds recommended gzip size.`));
      }
    }
  });
}

function copyPublicFolder() {
  fs.copySync(paths.appPublic, paths.appBuild, {
    dereference: true,
    filter: file => file !== paths.appHtml,
  });
}

async function build() {
  console.log(chalk.green('Creating an optimized production build...'));

  // 清空 build 文件夹
  fs.emptyDirSync(paths.appBuild);

  const previousSizes = await measureFileSizes(paths.appBuild);

  const compiler = webpack(config);

  return new Promise((resolve, reject) => {
    compiler.run((err, stats) => {
      if (err) return reject(err);

      const info = stats.toJson({ all: false, warnings: true, errors: true });

      if (info.errors.length) {
        info.errors.forEach(e => console.error(chalk.red(e.message || e)));
        return reject(new Error('Build failed with errors.'));
      }

      //if (info.warnings.length) {
      //  info.warnings.forEach(w => console.warn(chalk.yellow(w.message || w)));
      //}

      copyPublicFolder();
  
      printFileSizesAfterBuild(stats, previousSizes);

      console.log(chalk.green('\nBuild completed successfully!\n'));
      console.log(`Build folder: ${paths.appBuild}`);
	  console.log(`serve -s build`);
      resolve();
    });
  });
}

build().catch(err => {
  console.error(chalk.red(err.stack || err));
  process.exit(1);
});
