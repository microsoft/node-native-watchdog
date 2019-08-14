
console.log(`[${process.pid}] - reference process running...`);

setTimeout(() => {
	console.log(`[${process.pid}] - reference process exiting now!`);
}, 10000);
