





# Watchdog node module
Kills the current process when the JavaScript event loop is busy for more than a configurable amount of time. e.g. an accidental while true loop.

This is implemented by launching a separate thread from C++ which periodically checks if an interval timer installed in the JavaScript event loop is fired.

> Note: The node module does not distinguish a never-completing operation from a long running operation that will eventually complete. If the event loop appears to be stuck and does not fire timers for a configurable period of time, the node module will terminate the process.

## Installing

```sh
npm install native-watchdog
```

## Using

```javascript
var watchdog = require('native-watchdog');

// Configures the module to kill the current process if the event loop
// is unresponsive for more than 5s.
watchdog.start(5000);
```

The module will print to `stderr` the JavaScript call stack at the time it detected the JavaScript event loop to be unresponsive and will exit the process with code 87 (unused by node.js):
```
The module native-watchdog has detected that the event loop is unresponsive.
Here is the JavaScript stack trace:
======================================native-watchdog======================================
bad (/Users/alex/src/node-native-watchdog/test/test.js:16:13)
good (/Users/alex/src/node-native-watchdog/test/test.js:21:5)
Timeout._onTimeout (/Users/alex/src/node-native-watchdog/test/test.js:24:5)
ontimeout (timers.js:386:14)
tryOnTimeout (timers.js:250:5)
Timer.listOnTimeout (timers.js:214:5)
======================================native-watchdog======================================
The module native-watchdog will now terminate the process with exit code 87.
```

## Developing
 * `npm install -g node-gyp`
 * `node-gyp configure`
 * `node-gyp build` (for debugging use `node-gyp build --debug`)
 * `npm test` (for debugging change `index.js` to load the node module from the `Debug` folder and press `F5`)

## Contributing

This project welcomes contributions and suggestions.  Most contributions require you to agree to a
Contributor License Agreement (CLA) declaring that you have the right to, and actually do, grant us
the rights to use your contribution. For details, visit https://cla.microsoft.com.

When you submit a pull request, a CLA-bot will automatically determine whether you need to provide
a CLA and decorate the PR appropriately (e.g., label, comment). Simply follow the instructions
provided by the bot. You will only need to do this once across all repos using our CLA.

This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/).
For more information see the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or
contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional questions or comments.

## License
[MIT](https://github.com/Microsoft/node-native-watchdog/blob/master/LICENSE)
