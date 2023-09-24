module.exports = {
    module: {
        rules: [
            {
                test: /\.wasm/,
                type: 'asset/resource',
                generator: {
                    filename: 'wasm/[name]-[hash][ext][query]',
                },
            },
            {
                test: /\.md/,
                type: 'asset/resource',
                generator: {
                    filename: 'assets/doc/[name]-[hash][ext][query]',
                },
            },
        ],
    },
};
