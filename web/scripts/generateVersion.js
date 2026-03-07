'use strict';

const path = require('path');
const fs = require('fs');

const paths = require('../config/paths')

function gen () {
  const version = require(paths.appPackageJson).version;
  const date = new Date()
  const year = date.getFullYear()
  const month = `${date.getMonth() + 1}`
  const day = `${date.getDate()}`
  const hour = `${date.getHours()}`
  const minute = `${date.getMinutes()}`

  const dateTime = `${year}-${month.length < 2 ? `0${month}` : month}-${day.length < 2 ? `0${day}` : day} ${hour.length < 2 ? `0${hour}` : hour}:${minute.length < 2 ? `0${minute}` : minute}`

  const file = path.join(paths.appPublic, 'version.txt');
  fs.writeFileSync(file, `v${version}_${dateTime}`);
}

gen()
