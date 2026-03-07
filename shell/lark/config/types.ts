export interface HttpsConfig {
    key: string;
    pem: string;
}

export interface LarkConfig {
    app_id: string;
    app_secret: string;
}

export interface ServerConfig {
    host: string;
    httpPort: number;
    https: boolean;
    httpsConfig: HttpsConfig;
    userLogDir: string;
    adminWebDir: string;
    logLevel: 'debug' | 'info' | 'warn' | 'error';
    loggerName: string;
    hotScript: boolean;
    localHost: string;
    lark: LarkConfig;
}

export interface MasterConfig {
    id: string;
    host: string;
    port: number;
    client_ip: string;
    client_port: number;
}

export interface AppConfig {
    server: ServerConfig;
    master: MasterConfig;
}
