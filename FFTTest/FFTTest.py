import unicurses
import numpy as np
import math

stdscr = unicurses.initscr()
unicurses.cbreak()
unicurses.noecho()
unicurses.curs_set(0)
unicurses.keypad(stdscr, True)
LINES, COLS = unicurses.getmaxyx(stdscr)

samples = list()

dataPath = 'data\\sin440.bin'
#dataPath = 'c:\\users\\aaron_000\\desktop\\whistle, talk.bin'
with open(dataPath, 'rb', buffering=4096) as dataFile:
    while True:
        v = dataFile.read(1)
        if v == b'':
           break
        sample = v[0]
        v = dataFile.read(1)
        if v == b'':
           break
        sample |= v[0] << 8
        samples.append(sample)

startX = 0
scaleX = 1
scaleY = 1 / 32
bucketScaleY = 1 / 256
CHUNKSIZE = 512
SAMPLERATE = 1000

def calculateLevels(data):
    # Convert raw data to numpy array
    #data = unpack("%dh"%(len(data)/2),data)
    #data = np.array(data, dtype='h')
    # Apply FFT - real data so rfft used
    fourier = np.fft.rfft(data)
    # Remove last element in array to make it the same size as chunk
    #fourier = np.delete(fourier, len(fourier)-1)
    # Find amplitude
    power = np.log10(np.abs(fourier))**2
    # Arange array into 8 rows for the 8 bars on LED matrix
    #power = np.reshape(power, (8, CHUNKSIZE / 8))
    #matrix = np.int_(np.average(power, axis = 1) / 4)
    return power

# data is from 0-1
def drawData(char, data, x, y, width, height):
    skip = int(max(len(data), width) / width)
    i = 0
    for s in data[0:min(len(data), int(width) * skip):skip]:
        v = height - s * height
        if v >= 0 and v <= height:
            unicurses.mvaddstr(int(y + v), i + int(x), char)
        i += 1

while True:
    currentSamples = samples[startX:startX + int(min(len(samples), 512) * scaleX)]
    drawData('*', list((s / 1024) for s in currentSamples), 0, 0, COLS, LINES / 3)

    fftData = calculateLevels(currentSamples)
    drawData('^', list((s * scaleY) for s in fftData), 0, LINES / 3, COLS / 3, LINES * 2 / 3)

    bucketCount = int(math.log(len(fftData), 2))
    bucketted = list(range(bucketCount))
    bucketIndex = 0
    for bucketIndex in range(bucketCount):
        bucketStart = int(math.pow(2, bucketIndex))
        bucketEnd = bucketStart * 2 - 1
        for i in range(bucketIndex, min(bucketEnd, len(fftData))):
            bucketted[bucketIndex] += fftData[i]
    drawData('-', list((s * bucketScaleY) for s in bucketted), COLS / 3, LINES / 3, COLS / 3, LINES * 2 / 3)

    height = 16
    width = int(fftData.size / height)
    fftData = fftData[0:width * height]
    power = np.reshape(fftData, (height, width))
    #matrix = np.int_(np.average(power, axis = 1) / 4)
    #drawData('x', list((s * bucketScaleY) for s in matrix), COLS * 2 / 3, LINES / 3, COLS / 3, LINES * 2 / 3)

    for j in range(len(power)):
        for i in range(len(power[j])):
            v = power[j][i]
            c = chr(ord('0') + int(v / 5))
            unicurses.mvaddstr(int(LINES * 2 / 3 + height - j) - 5, int(COLS * 2 / 3 + i), c)

    unicurses.refresh()
    key = unicurses.getch()

    if key == unicurses.KEY_LEFT:
        if startX > 0:
            startX -= COLS
            unicurses.clear()
    elif key == unicurses.KEY_RIGHT:
        startX += COLS
        unicurses.clear()
    elif key == unicurses.KEY_UP:
        scaleY *= 2
        unicurses.clear()
    elif key == unicurses.KEY_DOWN:
        scaleY /= 2
        unicurses.clear()
    elif unicurses.keyname(key) == 'CTL_UP':
        bucketScaleY *= 2
        unicurses.clear()
    elif unicurses.keyname(key) == 'CTL_DOWN':
        bucketScaleY /= 2
        unicurses.clear()
    elif unicurses.keyname(key) == '[':
        scaleX *= 2
        unicurses.clear()
    elif unicurses.keyname(key) == ']':
        scaleX = max(scaleX / 2, 1)
        unicurses.clear()
    elif unicurses.keyname(key)[0] == '^' and unicurses.keyname(key)[1] == 'C':
        break
    else:
        unicurses.mvaddstr(0, 0, unicurses.keyname(key))
        unicurses.refresh()
   