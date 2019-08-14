
const cp = require('child_process');
const path = require('path');

// Start a reference process
const ref_child = cp.fork(path.join(__dirname, 'reference_child.js'));

const bad_child = cp.fork(path.join(__dirname, 'bad_child.js'), [ref_child.pid], {
	detached: true
});

console.log(`reference pid - ${ref_child.pid}, bad child pid - ${bad_child.pid}`);

let refchildStopTime = 0;
setInterval(function() {
	const refchild = isRunning(ref_child.pid);
	const badchild = isRunning(bad_child.pid);

	if (!refchild) {
		if (refchildStopTime === 0) {
			refchildStopTime = Date.now();
		}

		// the reference process is dead
		if (!badchild) {
			// great! the bad child has died
			console.log(`[TEST PASSED] - The bad child process has exited!`);
			process.exit(0);
		}

		// the bad child is running
		let runningTime = Date.now() - refchildStopTime;
		if (runningTime < 10000) {
			// give 10seconds...
			console.log(`[WAIT] - The reference process has exited ${Math.floor(runningTime / 1000)}s ago, the bad child should exit soon...`);
		} else {
			console.log(`[TEST FAILED] - The reference process has exited ${Math.floor(runningTime / 1000)}s ago, the bad child should have exited already!!! You should kill it manually to not hog your machine -- PID of bad child is ${bad_child.pid}`);
		}
	} else {
		console.log(`[WAIT] - Both processes are running...`);
	}
}, 1000);

function isRunning(pid) {
	try {
		process.kill(pid, 0); // throws an exception if the process doesn't exist anymore
		return true;
	} catch (e) {
		return false;
	}
}
