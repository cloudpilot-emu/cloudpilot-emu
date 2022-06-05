const { optimize } = require('svgo');

module.exports = function skinLoader(source) {
    const optimized = optimize(source, {
        multipass: true,
        plugins: [{ name: 'preset-default', params: { overrides: { removeViewBox: false } } }],
    }).data;

    const json = JSON.stringify(optimized)
        .replace(/\u2028/g, '\\u2028')
        .replace(/\u2029/g, '\\u2029');

    return `export default ${json};`;
};
