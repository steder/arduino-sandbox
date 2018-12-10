import time

import audioio
import board
import digitalio
import neopixel

RED =   (16, 0, 0)
YELLOW = (16, 16, 0)
WHITE = (16, 16, 16)
GREEN = (0, 16, 0)
BLACK = (0, 0 , 0)

leftButton = digitalio.DigitalInOut(board.D4)
rightButton = digitalio.DigitalInOut(board.D5)
leftButton.direction = digitalio.Direction.INPUT
leftButton.pull = digitalio.Pull.DOWN
rightButton.direction = digitalio.Direction.INPUT
rightButton.pull = digitalio.Pull.DOWN

# switch = DigitalInOut(board.D2)
# switch = DigitalInOut(board.D5)  # For Feather M0 Express, Feather M4 Express
switch = digitalio.DigitalInOut(board.D7)  # For Circuit Playground Express
switch.direction = digitalio.Direction.INPUT
switch.pull = digitalio.Pull.UP

led = digitalio.DigitalInOut(board.D13)
led.direction = digitalio.Direction.OUTPUT

pixels = neopixel.NeoPixel(board.NEOPIXEL, 10, brightness = 0.4)
pixels.fill(BLACK)


print("Hello World!")

# red ring in honor of oracle:
for i in range(10):
    pixels[i] = (16 * (i+1), 0, 0)
time.sleep(1)


def checkButtons():
    if rightButton.value:
        pixels.brightness -= 0.1

    if leftButton.value:
        pixels.brightness += 0.1


while True:
    sleep_time = 0.1
    led.value = not switch.value
    time.sleep(sleep_time)


    # n * i * sleep_time = 10 seconds per block here

    # chasing green and red with yellows in the middle
    for n in range(10):
        for i in range(10):
            pixels[(i-1) % 10] = BLACK
            pixels[i] = RED
            pixels[(i+1) % 10] = WHITE
            pixels[(i+2) % 10] = WHITE
            pixels[(i+3) % 10] = GREEN
            time.sleep(sleep_time)
            checkButtons()

    # pulsating yellow and white with slower rotation
    for n in range(30):
        for i in range(10):
            if i % 2 == ((n % 2) == 1):
                pixels[i] = YELLOW
            else:
                pixels[i] = WHITE
        for j in range(10):
            if j < 5:
                pixels.brightness += .1
            else:
                pixels.brightness -= .1
            time.sleep(sleep_time)
            checkButtons()
        time.sleep(sleep_time)
        checkButtons()

    # flash the sides green and red
    for n in range(10 * 30):
        if n % 2 == 0:
            LOW_COLOR, HIGH_COLOR = GREEN, RED
        else:
            LOW_COLOR, HIGH_COLOR = RED, GREEN
        for i in range(10):
            if i < 5:
                pixels[i] = LOW_COLOR
            else:
                pixels[i] = HIGH_COLOR
        time.sleep(sleep_time)
        checkButtons()

    # rotating red and greens:
    # evens are green, odds are red
    for n in range(10 * 30):
        for i in range(10):
            if i % 2 == ((n % 2) == 1):
                pixels[i] = GREEN
            else:
                pixels[i] = RED
        time.sleep(sleep_time)
        checkButtons()

    # flash the *other* sides green and red
    for n in range(10 * 30):
        if n % 2 == 0:
            LOW_COLOR, HIGH_COLOR = GREEN, RED
        else:
            LOW_COLOR, HIGH_COLOR = RED, GREEN
        for i in range(10):
            if i in [7, 8, 9, 0, 1]:
                pixels[i] = LOW_COLOR
            else:
                pixels[i] = HIGH_COLOR
        time.sleep(sleep_time)
        checkButtons()

    # rotating candy cane
    for n in range(10 * 30):
        for i in range(10):
            if i % 2 == ((n % 2) == 1):
                pixels[i] = WHITE
            else:
                pixels[i] = RED
        time.sleep(sleep_time)
        checkButtons()
