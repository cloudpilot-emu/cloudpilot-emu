const path = require('path');

module.exports = {
    module: {
        rules: [
            {
                test: /.*\.svg/,
                use: {
                    loader: path.resolve(path.join(__dirname, 'webpack', 'svg-loader.js')),
                },
            },
        ],
    },
};
