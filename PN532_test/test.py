import servo
import numpy as np


# generates a random testcase
def generate_case():
    expected_result = np.random.randint(0, 1, 3)
    servo.set_cards = expected_result
    expected_result[3] = expected_result[2]
    return expected_result


print(generate_case())
