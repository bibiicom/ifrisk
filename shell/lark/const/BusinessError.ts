import {ErrorCode} from './ErrorCode';

export class BusinessError extends Error {
    public key: number;

    constructor(key: number, message: string) {
        super(message);
        this.name = 'BusinessError';
        this.key = key;
    }

    static new(key: number, message?: string) {
        return new BusinessError(key, message || ErrorCode.enum[key]);
    }

    static newMsg(message: string) {
        return new BusinessError(1, message);
    }

}