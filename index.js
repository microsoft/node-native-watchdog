/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

var watchdog = require('./build/Release/watchdog');

var hasStarted = false;

exports.start = function(timeout) {
    if (hasStarted) {
        return;
    }
    hasStarted = true;
    watchdog.start(timeout);
    setInterval(function() {
        // let the C++ side know that the event loop is alive.
        watchdog.ping();
    }, 1000);
};

exports.exit = function(code) {
    watchdog.exit(code || 0);
}
