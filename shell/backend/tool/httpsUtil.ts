import path = require('path');
import fs = require('fs');
import https = require('https');
import constants = require('constants');
import {RequestListener} from "node:http";

export function enableHandlerHttps(handler: RequestListener, httpsConfig: {
    keyPath: string;
    certPath: string;
}) {
    const options = {
        key: fs.readFileSync(path.resolve(httpsConfig.keyPath)),
        cert: fs.readFileSync(path.resolve(httpsConfig.certPath)),
        secureOptions: constants.SSL_OP_NO_SSLv2 | constants.SSL_OP_NO_TLSv1 | constants.SSL_OP_NO_TLSv1_1 | constants.SSL_OP_NO_TLSv1_2
    };
    return https.createServer(options, handler);
}
