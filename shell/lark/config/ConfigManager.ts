import * as fs from 'fs';
import * as path from 'path';
import {parse} from 'json5';
import {AppConfig, MasterConfig, ServerConfig} from './types';

class ConfigManager {
    private config: AppConfig;
    private readonly configDir: string;
    private readonly env: string;

    public constructor() {
        this.configDir = path.join(__dirname);
        this.env = this.getCurrentEnv();
        this.config = this.load();
    }

    private getCurrentEnv(): string {
        return process.env.NODE_ENV;
    }

    private loadConfigFile(filename: string): any {
        const filePath = path.join(this.configDir, filename);

        if (!fs.existsSync(filePath)) {
            return null;
        }

        try {
            const content = fs.readFileSync(filePath, 'utf-8');
            return parse(content);
        } catch (error) {
            throw new Error(`Failed to load config file ${filename}: ${error.message}`);
        }
    }

    private mergeConfigs(base: any, override: any): any {
        if (!override) {
            return base;
        }

        const result = {...base};

        for (const key in override) {
            if (override.hasOwnProperty(key)) {
                if (typeof override[key] === 'object' && override[key] !== null && !Array.isArray(override[key])) {
                    result[key] = this.mergeConfigs(base[key] || {}, override[key]);
                } else {
                    result[key] = override[key];
                }
            }
        }

        return result;
    }

    private resolveEnvVars(config: any): any {
        if (typeof config === 'string') {
            return config.replace(/\$\{([^}]+)\}/g, (match, varName) => {
                const envValue = process.env[varName];
                return envValue !== undefined ? envValue : match;
            });
        }

        if (Array.isArray(config)) {
            return config.map(item => this.resolveEnvVars(item));
        }

        if (typeof config === 'object' && config !== null) {
            const result: any = {};
            for (const key in config) {
                if (config.hasOwnProperty(key)) {
                    result[key] = this.resolveEnvVars(config[key]);
                }
            }
            return result;
        }

        return config;
    }

    private load(): AppConfig {
        const defaultConfig = this.loadConfigFile('default.json5');

        if (!defaultConfig) {
            throw new Error('default.json5 not found');
        }

        const envConfig = this.loadConfigFile(`${this.env}.json5`);
        const mergedConfig = this.mergeConfigs(defaultConfig, envConfig);
        const finalConfig = this.resolveEnvVars(mergedConfig);

        this.validateConfig(finalConfig);

        return finalConfig as AppConfig;
    }

    private validateConfig(config: any): void {
        if (!config.server) {
            throw new Error('Invalid config: server section is missing');
        }

        const server = config.server;
        if (!server.host || typeof server.host !== 'string') {
            throw new Error('Invalid config: server.host is missing or invalid');
        }

        if (!server.httpPort || typeof server.httpPort !== 'number') {
            throw new Error('Invalid config: server.httpPort is missing or invalid');
        }

        if (!server.lark || !server.lark.app_id || !server.lark.app_secret) {
            console.warn('Warning: Lark credentials are not configured');
        }
    }

    public getAll(): AppConfig {
        return this.config;
    }

    public getServer(): ServerConfig {
        return this.config.server;
    }

    public getMaster(): MasterConfig {
        return this.config.master;
    }

    public get(path: string): any {
        const keys = path.split('.');
        let value: any = this.config;

        for (const key of keys) {
            if (value && typeof value === 'object' && key in value) {
                value = value[key];
            } else {
                return undefined;
            }
        }

        return value;
    }

    public reload(): void {
        this.config = this.load();
    }

    public getEnv(): string {
        return this.env;
    }
}

export default ConfigManager;
