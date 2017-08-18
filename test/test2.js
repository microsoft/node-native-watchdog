/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

var watchdog = require('../index.js');

watchdog.start(5000);

function fib(n) {
    if (n === 0) { return 1; }
    if (n === 1) { return 1; }
    return fib(n-1) + fib(n-2);
}

// Simulate busy work, but which completes faster than the time limit
setInterval(function() {
    var start = Date.now();
    console.log(`doing usual work...`);
    fib(42);
    console.log(`...finished doing work after ${Date.now() - start} ms.`);
}, 1000);
