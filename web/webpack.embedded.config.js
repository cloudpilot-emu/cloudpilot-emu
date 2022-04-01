const path = require('path');
const CopyPlugin = require('copy-webpack-plugin');

module.exports = (env, argv) => ({
    entry: './embedded/src/index.ts',
    devtool: argv.mode === 'development' ? 'eval-source-map' : 'source-map',
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
        alias: {
            '@common': path.resolve(__dirname, './common'),
        },
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
                { from: path.resolve(__dirname, '../src/cloudpilot_web.wasm'), to: '.' },
            ],
        }),
    ],
    performance: {
        maxAssetSize: 1 * 1024 * 1024,
        maxEntrypointSize: 1 * 1024 * 1024,
    },
    devServer: {
        static: {
            directory: path.join(__dirname, 'embedded/public'),
        },
        compress: true,
        port: 9000,
    },
});
