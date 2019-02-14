import board
import digitalio
import time
#import neopixel
import busio
import adafruit_pca9685
import adafruit_motor
import adafruit_motor.servo

i2c = busio.I2C(board.SCL, board.SDA)
pca = adafruit_pca9685.PCA9685(i2c)
pca.frequency = 50
servo1 = adafruit_motor.servo.Servo(pca.channels[0])

buttonD4 = digitalio.DigitalInOut(board.D4)
buttonD4.direction = digitalio.Direction.INPUT
buttonD4.pull = digitalio.Pull.DOWN
buttonD5 = digitalio.DigitalInOut(board.D5)
buttonD5.direction = digitalio.Direction.INPUT
buttonD5.pull = digitalio.Pull.DOWN

#pixels = neopixel.NeoPixel(board.NEOPIXEL, 10, brightness=0.3, auto_write=False)
#pixels.fill((0, 0, 0))
#pixels.show()

servoIndicies = {
    'Clavicle.R': 0,
    'Shoulder.R': 1,
    'Upper.R': 2,
    'Lower.R': 3,
    'Clavicle.L': 4,
    'Shoulder.L': 5,
    'Upper.L': 6,
    'Lower.L': 7,
    'Torso': 8
}
#servos = {
#    'Clavicle.R': 330,
#    'Shoulder.R': 320,
#    'Upper.R': 345,
#    'Lower.R': 290,
#    'Clavicle.L': 330,
#    'Shoulder.L': 320,
#    'Upper.L': 345,
#    'Lower.L': 290
#}

def loadAnimations():
    actions = {}
    with open('animations.txt', 'r') as animationsFile:
        for line in animationsFile:
            if line.startswith('Action '):
                actionName = line[7:].strip()
                print('Action {}'.format(actionName))
                currentAction = []
            if line.startswith('EndAction'):
                actions[actionName] = currentAction
                del currentAction

            if line.startswith('  Frame'):
                #print(line.strip())
                currentFrame = [0]*len(servoIndicies)
            if line.startswith('  EndFrame'):
                currentAction.append(currentFrame)
                del currentFrame

            if line.startswith('    Bone '):
                split = line[9:].split(':')
                boneName = split[0]
                boneIndex = servoIndicies[boneName]
                boneValue = int(split[1][1:])
                #print('    Bone {}: {}'.format(boneIndex, boneValue))
                currentFrame[boneIndex] = boneValue
    return actions

actions = loadAnimations()

for actionName, frames in actions.items():
    print("Action {} ({})".format(actionName, len(frames)))
    for frame in frames:
        print("  Frame")
        for i, boneValue in enumerate(frame):
            print("  Bone {}: {}".format(i, boneValue))

index = 0
running = False

led = digitalio.DigitalInOut(board.D13)
led.direction = digitalio.Direction.OUTPUT

while True:
    led.value = True
    time.sleep(0.5)
    led.value = False
    time.sleep(0.5)

    #pixels.fill((0, 0, 0))

    if running:
        # servo1.angle = servoStops[index]
        #pixels[index] = (0x10, 0x10, 0x10)
        #pixels.show()
        index = (index + 1) % 5
        # index = (index + 1) % len(servoStops)

    #pixels.show()

    if buttonD4.value:
        running = True

    if buttonD5.value:
        running = False
