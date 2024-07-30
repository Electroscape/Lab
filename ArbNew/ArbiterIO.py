try:
    from pcf8574 import PCF8574
except ImportError:
    from PCF_dummy import PCF8574
from time import thread_time, sleep
from arbiter_config import arbiter_address_config

# @TODO: make input vs output pcf list/defenition, just a variable now

# @TODO: make input vs output pcf list/defenition, just a variable now


class ArbiterIO:
    def __init__(self):
        self.pcf_inputs = []
        self.pcf_outputs = []
        self.__pcf_init()

    def __pcf_init(self):

        for index, pcf_add in enumerate(arbiter_address_config):
            print(pcf_add)
            self.pcf_inputs.append(PCF8574(1, pcf_add))
            self.pcf_outputs.append(PCF8574(0, pcf_add))
            for pin in range(0, 8):
                self.pcf_inputs[index].port[pin] = True
                self.pcf_outputs[index].port[pin] = True

    # currently only the inputs
    def __pcf_pullup(self):
        for pcf in self.pcf_inputs:
            for pin in range(0, 8):
                pcf.port[pin] = True

    def pcf_has_input(self, pcf):
        for pin_index in range(0, 8):
            pin = 7 - pin_index
            try:
                ret = bool(pcf.port[pin])
                if not ret:
                    sleep(0.02)
                    return True
            except IOError:
                pass
                # print(f"error reading PCF {pcf_index}")
        return False

    def read_pcf(self, pcf_index):
        pcf = self.pcf_inputs[pcf_index]
        result = 0
        if not self.pcf_has_input(pcf):
            return result
        for pin_index in range(0, 8):
            pin = 7 - pin_index
            try:
                ret = bool(pcf.port[pin])
                if not ret:
                    result += 1 <<  pin_index
            except IOError:
                print(f"error reading PCF {pcf_index}")
        return result

    def write_pcf(self, pcf_index, value):
        for pin in range(0, 8):
            pin_value = 1 << (7 - pin)
            # print(f'{pin}: {pin_value}')
            output = not value & pin_value
            try:
                self.pcf_outputs[pcf_index].port[pin] = output
            except IOError:
                print("IOError")
                return False

    def get_inputs(self):
        self.__pcf_pullup()
        inputs = []
        for index, _ in enumerate(self.pcf_inputs):
            pin_value = self.read_pcf(index)
            if pin_value > 0:
                inputs.append((index, pin_value))
        return inputs


class CooldownHandler:
    def __init__(self):
        self.cooldowns = set()

    def is_input_on_cooldown(self, input_pcf, event_pcf_value):
        for cooldown in self.cooldowns:
            if input_pcf == cooldown[0] and event_pcf_value == cooldown[1]:
                if cooldown[2] > thread_time():
                    # print("GPIO is on cooldown")
                    return True
        return False

