import RPi.GPIO as GPIO
from time import sleep

GPIO.setwarnings(False)

GPIO.setmode(GPIO.BOARD)
motorCycle = 50
read_angle = 15
remove_angle = 105
pwm_pins = [12, 13, 18]
pwm_array = []
current_angles = [0, 0, 0, 0]


def init_pwms():
    for pin in pwm_pins:
        GPIO.setup(pin, GPIO.OUT)
        current_pwm = GPIO.PWM(pin, motorCycle)
        current_pwm.start(0)
        pwm_array.append(current_pwm)


# angle is from 0 to 180 deg
def get_dc_from_angle(rot):
    rot = (rot / 180) * 12.5
    result = max(0.1, rot)
    result = min(12.5, result)
    return result


def cleanup():
    set_cards((0,0,0,0))
    for pwm in pwm_array:
        pwm.stop()
    delay(0.1)
    GPIO.cleanup()
    print("end")


# only supports 3 pwms so only 3 cards can move independent,
# card 4 will just be mirrored from 3
def set_cards(card_statuses):
    for i, card in enumerate(card_statuses):
        angle = remove_angle
        if card:
            angle = read_angle
        if angle != current_angles[i]:
            pwm_array[i].ChangeDutyCycle(get_dc_from_angle(angle))
    sleep(1)


def main():
    init_pwms()

    angle = 0
    try:
        while True:
            if angle > 20:
                angle = 0
            else:
                angle = 50
            for pwm in pwm_array:
                pwm.ChangeDutyCycle(get_dc_from_angle(angle))
            sleep(1)
    except KeyboardInterrupt:
        pass
        print("interrupt")
        cleanup()
        exit()


# allows me to import into test.py without running the main to test functions
if __name__ == "__main__":
    main()
else:
    init_pwms()


