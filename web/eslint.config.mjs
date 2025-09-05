// @ts-check

import { defineConfig } from 'eslint/config';
import prettier from 'eslint-config-prettier';
import angular from 'angular-eslint';
import tseslint from 'typescript-eslint';
import eslint from '@eslint/js';
import { default as parser } from '@typescript-eslint/parser';

export default defineConfig([
    {
        files: ['**/*.ts'],
        ignores: ['xterm.js'],
        extends: [eslint.configs.recommended, prettier, tseslint.configs.recommended, angular.configs.tsRecommended],
        processor: angular.processInlineTemplates,
        languageOptions: {
            parser,
            parserOptions: { project: 'tsconfig.eslint.json' },
        },
        rules: {
            '@angular-eslint/component-selector': [
                'error',
                {
                    type: 'element',
                    prefix: 'app',
                    style: 'kebab-case',
                },
            ],
            '@angular-eslint/directive-selector': [
                'error',
                {
                    type: 'attribute',
                    prefix: 'app',
                    style: 'camelCase',
                },
            ],
            '@typescript-eslint/member-delimiter-style': [
                'off',
                {
                    multiline: {
                        delimiter: 'none',
                        requireLast: true,
                    },
                    singleline: {
                        delimiter: 'semi',
                        requireLast: false,
                    },
                },
            ],
            '@typescript-eslint/no-non-null-assertion': 'off',
            '@typescript-eslint/semi': ['off', null],
            curly: ['error', 'multi-line'],
            'no-bitwise': 'off',
            'space-before-function-paren': 'off',
            eqeqeq: 'error',
            '@typescript-eslint/no-unused-vars': [
                'error',
                {
                    ignoreRestSiblings: true,
                },
            ],
            '@typescript-eslint/no-floating-promises': 'error',
            '@angular-eslint/component-class-suffix': [
                'error',
                {
                    suffixes: ['Page', 'Component'],
                },
            ],
            '@angular-eslint/prefer-standalone': 'off',
            '@angular-eslint/prefer-inject': 'off',
        },
    },
    {
        files: ['pwa/**/*.ts'],
        languageOptions: {
            parserOptions: { project: 'pwa/tsconfig.app.json' },
        },
    },
    {
        files: ['**/*.html'],
        // @ts-ignore
        extends: [angular.configs.templateRecommended],
    },
]);
