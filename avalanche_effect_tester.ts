import { spawn } from 'child_process'

const random = (max: number) => Math.floor(Math.random() * max)
const genByte = () => random(256)
const gen16Bytes = () => new Uint8Array(16).map(genByte)

const alterArray = (
	originalArray: Uint8Array
) => {
	const alteredArray = new Uint8Array(originalArray)
	const randomIndex = random(16)

	alteredArray[randomIndex] = genByte()

	return alteredArray
}

const prepareDataForEncryption = (
	plainText: Uint8Array,
	key: Uint8Array
) => {
	const array = new Uint8Array(36)

	array[0] = 0x33					// '3'
	array[1] = 0x0A					// '\n'
	array.set(plainText, 2)	// plainText
	array[18] = 0x0A				// '\n'
	array.set(key, 19)			// key
	array[35] = 0x0A				// '\n'

	return array
}

const encrypt = (
	plainText: Uint8Array,
	key: Uint8Array
) => new Promise<Uint8Array>(resolve => {
	const process = spawn('./5')

	// Resolve when stdout sends data

	process.stdout.on('data', (data: Buffer) => resolve(new Uint8Array(data)))

	// Send the initial data to stdin

	const data = prepareDataForEncryption(plainText, key)

	process.stdin.write(data)
})

const calcAvalancheEffect = (
	cipherText1: Uint8Array,
	cipherText2: Uint8Array
) => {
	let count = 0

	for (let i = 0; i < 16; i++) {
		for (let j = 0; j < 8; j++) {
			// Check if the j-th bit of the i-th byte differ

			if ((cipherText1[i] ^ cipherText2[i]) & (1 << j)) {
				count++
			}
		}
	}

	return count / 128
}

const endRuns = () => {
	console.log('All', testCases, 'runs finished')
	console.log('The average avalanche effect is:', avalancheEffectSum / testCases)
	console.log(`The program took ${ Date.now() - startingTime }ms to complete`)
}

const handleRunOutput = (
	i: number,
	run1: Uint8Array,
	run2: Uint8Array
) => {
	console.log('Run', i, 'finished')
	processedOutputs++

	// Add the avalanche effect of this run to the avalanche effect sum

	const avalancheEffect = calcAvalancheEffect(run1, run2)
	avalancheEffectSum += avalancheEffect

	// End the program when we processed all outputs

	if (processedOutputs == testCases) {
		endRuns()
	}
}

const testCases = 1E3
let avalancheEffectSum = 0
let processedOutputs = 0
let startingTime = Date.now()

for (let i = 0; i < testCases; i++) {
	console.log('Run', i, 'started')

	// Generate a random plain text

	const plainText = gen16Bytes()

	// Slightly alter the plain text

	const alteredPlainText = alterArray(plainText)

	// Generate a random key

	const key = gen16Bytes()

	// Encrypt the normal plain text

	const run1 = encrypt(plainText, key)

	// Encrypt the slightly altered plain text

	const run2 = encrypt(alteredPlainText, key)

	// Handle run output

	Promise.all([ run1, run2 ]).then(values => handleRunOutput(i, values[0], values[1]))
}