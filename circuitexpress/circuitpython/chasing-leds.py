import time
import board
import neopixel
import audioio

RED =   (16, 0, 0)
YELLOW = (16, 16, 0)
GREEN = (0, 16, 0)
BLACK = (0, 0 , 0)

pixels = neopixel.NeoPixel(board.NEOPIXEL, 10, brightness = 0.1)
pixels.fill(BLACK)

print("Hello World!")

while True:
    for i in range(10):
        pixels[(i-1) % 10] = BLACK
        pixels[i] = RED
        pixels[(i+1) % 10] = YELLOW
        pixels[(i+2) % 10] = YELLOW
        pixels[(i+3) % 10] = GREEN
        time.sleep(0.1)
