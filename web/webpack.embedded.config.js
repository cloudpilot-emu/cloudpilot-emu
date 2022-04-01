const path = require('path');
const CopyPlugin = require('copy-webpack-plugin');

module.exports = {
    entry: './embedded/src/index.ts',
    devtool: 'eval-source-map',
    module: {
        rules: [
            {
                test: /\.ts$/,
                use: {
                    loader: 'ts-loader',
                    options: {
                        configFile: path.resolve(__dirname, 'tsconfig.embedded.json'),
                    },
                },
                exclude: /node_modules/,
            },
        ],
    },
    resolve: {
        extensions: ['.ts', '.js'],
    },
    output: {
        path: path.resolve(__dirname, 'build-embedded'),
        filename: 'cloudpilot-emu.js',
        library: {
            name: 'cloudpilot',
            type: 'umd',
        },
    },
    plugins: [
        new CopyPlugin({
            patterns: [
                { from: 'embedded/public', to: '.' },
                { from: path.resolve(__dirname, '../src/cloudpilot_web.js'), to: '.' },
                { from: path.resolve(__dirname, '../src/cloudpilot_web.wasm'), to: '.' },
            ],
        }),
    ],
    devServer: {
        static: {
            directory: path.join(__dirname, 'embedded/public'),
        },
        compress: true,
        port: 9000,
    },
};
