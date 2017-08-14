/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

 var watchdog = require('../index.js');

watchdog.start(5000);

// Simulate usual work
setInterval(function() {
    console.log(`doing usual work...`);
}, 1000);

function bad() {
    console.log(`entering while true loop that will block the event loop for more than 5s`)
    while (true) {
    }
}
function good() {
    bad();
}
setTimeout(function() {
    good();
}, 3000);
