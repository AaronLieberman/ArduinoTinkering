import math

samplingRate = 2000
lengthInSeconds = 1
frequency = 440 # middle A (iirc)

samples = list()
for j in range(4):
    pointScale = samplingRate / frequency / (j + 1) * 2 * math.pi
    for i in range(samplingRate * lengthInSeconds):
        v = int((math.sin(i / pointScale) + 1) * 512)
        samples.append(v & 255)
        samples.append((v >> 8) & 255)

dataPath = 'c:\\users\\aaron_000\\desktop\\sin440.bin'
with open(dataPath, 'wb', buffering=4096) as dataFile:
    dataFile.write(bytes(samples))
