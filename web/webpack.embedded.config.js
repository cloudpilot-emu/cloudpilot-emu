const path = require('path');
const { execSync } = require('child_process');
const CopyPlugin = require('copy-webpack-plugin');
const webpack = require('webpack');
const pkg = require('./package.json');

function getGitRev() {
    const rev = execSync('git rev-parse --short HEAD', { encoding: 'utf-8' }).trimEnd();

    if (!/^[0-9a-f]{7,8}$/.test(rev)) throw new Error(`unable to determine git revision; command returned ${rev}`);

    return rev;
}

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
            '@native': path.resolve(__dirname, '../src/cloudpilot'),
            '@native-common': path.resolve(__dirname, '../src/common'),
            '@pwa': path.resolve(__dirname, './pwa/src/app'),
            '@embedded': path.resolve(__dirname, './embedded/src'),
            '@native-fstools': path.resolve(__dirname, './src/fstools'),
            '@native-uarm': path.resolve(__dirname, './src/uarm'),
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
                { from: path.resolve(__dirname, '../src/cloudpilot/cloudpilot_web.wasm'), to: '.' },
            ],
        }),
        new webpack.EnvironmentPlugin({
            VERSION:
                argv.mode === 'development'
                    ? 'dev'
                    : env['RELEASE'] || process.env['CP_RELEASE']
                      ? pkg.version
                      : `${pkg.version}-${getGitRev()} (preview)`,
        }),
    ],
    performance: {
        maxAssetSize: 3 * 1024 * 1024,
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
