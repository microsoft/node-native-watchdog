
const nw = require('../index.js');
const parentpid = parseInt(process.argv[2]);

nw.start(parentpid);

console.log(`[${process.pid}] - bad process entering endless loop...`);

while (true) {
	; // intentional
}
