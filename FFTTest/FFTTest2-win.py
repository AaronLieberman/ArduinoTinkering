import unicurses
import numpy as np
import math
import wave
import struct
import time

stdscr = unicurses.initscr()
unicurses.cbreak()
unicurses.noecho()
unicurses.curs_set(0)
unicurses.keypad(stdscr, True)
LINES, COLS = unicurses.getmaxyx(stdscr)

height = 16

def drawData(data, x, y):
    for i in range(len(data)):
        v = data[i]
        for j in range(height):
            c = ' '
            if (j <= v - 1):
                c = '#'
            unicurses.mvaddstr(y + height - 1 - j, x + i, c)

# Initialize matrix
matrix = [0, 0, 0, 0, 0, 0, 0, 0]
power = []
weighting = [2, 2, 8, 8, 16, 32, 64, 64] # Change these according to taste

# Set up audio
wavfile = wave.open('data/rebel-theme.wav','r')
sampleRate = wavfile.getframerate()
noChannels = wavfile.getnchannels()
chunk = 4096 # Use a multiple of 8

# Return power array index corresponding to a particular frequency
def piff(val):
    return int(2 * chunk * val / sampleRate)
   
def calculateLevels(sampleData, chunk):
    global matrix
    # Convert raw data (ASCII string) to numpy array
    data = struct.unpack("%dh" % (len(sampleData) / 2), sampleData)
    data = np.array(data, dtype='h')
    # Apply FFT - real data
    fourier = np.fft.rfft(data)
    # Remove last element in array to make it the same size as chunk
    fourier = np.delete(fourier, len(fourier) - 1)
    # Find average 'amplitude' for specific frequency ranges in Hz
    power = np.abs(fourier)
    low = 0
    high = 156.25
    for i in range(8):
        matrix[i] = int(np.mean(power[piff(low) : piff(high) : 1]))
        low = high
        high *= 2
    # Tidy up column values for the LED matrix
    matrix = np.divide(np.multiply(matrix, weighting), 8000000 / height)
    # Set floor at 0 and ceiling at 8 for LED matrix
    matrix = matrix.clip(0, height)
    return matrix

lastTime = time.clock()
frameTime = chunk / sampleRate

data = wavfile.readframes(chunk)
while data != '' and len(data) > 0:
    matrix = calculateLevels(data, chunk)
    drawData(matrix, 0, 0)
    data = wavfile.readframes(chunk)

    unicurses.refresh()
    #key = unicurses.getch()

    now = time.clock()
    time.sleep(max(frameTime - (now - lastTime), 0))
    lastTime = now
