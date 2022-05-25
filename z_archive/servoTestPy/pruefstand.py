import servo
import numpy as np


def main():
    status = True
    while True:
        soll_status = np.random.randint(0, 1, 3)
        servo.set_cards(soll_status)
        soll_status[4] = soll_status[3]

