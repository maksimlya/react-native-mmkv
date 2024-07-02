const path = require('path');
const pak = require('../package.json');

module.exports = {
  presets: ['module:metro-react-native-babel-preset'],
  plugins: [
    ["@babel/plugin-transform-class-properties", { "loose": true }],
    ["@babel/plugin-transform-private-methods", { "loose": true }],
    ["@babel/plugin-transform-private-property-in-object", { "loose": true }],
    [
      'module-resolver',
      {
        alias: {
          [pak.name]: path.join(__dirname, '..', pak.source),
        },
      },
    ],
  ],
};
