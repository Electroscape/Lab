import requests
import subprocess
from enum import IntEnum

# http://www.compciv.org/guides/python/fundamentals/dictionaries-overview/
# defaults?

# these are the pcf addresses, first 3 are Arbiter -> Brain as outputs
# last 3 are Brain -> Arbiter inputs

# for rev 0.1
# [0x38, 0x39, 0x3A, 0x3C, 0x3D, 0x3E]
# for rev 0.2 and onwards
# [0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D]

#
laserlock_out_pcf = 0
# also used for the cleanroom rigger
airlock_out_pcf = 1
lab_light_out_pcf = 2

# inputs
laserlock_in_pcf = 4
# If we need more inputs this is the prime candidate to consolidate with the above
laserlock_in_2_pcf = 3
airlock_in_pcf = 5
analyzer_in_pcf = laserlock_in_2_pcf
locker_in_pcf = laserlock_in_2_pcf      # used for the service enable/disable
locker_out_pcf = airlock_out_pcf
dispenser_out_pcf = lab_light_out_pcf


sound = "sound"
is_fx = "is_fx"
sound_id = "id"

pcf_in = "pcf_in"
# if a specific pin must be set to high in order to trigger, only works on non-binary
pcf_in_add = "pcf_in_add"
pcf_out = "pcf_out"
pcf_out_add = "pcf_out_add"

# event triggered from FE
trigger_cmd = "trigger_cmd"
# may not always be required
trigger_msg = "trigger_msg"
# event triggering FE

fe_cb = "fe_cb"
fe_cb_tgt = "tgt"
fe_cb_cmd = "cmd"
fe_cb_msg = "msg"

event_script = "script"
event_condition = "condition"
event_delay = "delay"
event_next_qeued = "event_next"


class AirlockOut(IntEnum):
    test_7 = 1 << 7
    test_6 = 1 << 6
    test_5 = 1 << 5
    test_4 = 1 << 4
    test_3 = 1 << 3
    test_2 = 1 << 2
    test_1 = 1 << 1
    test_0 = 1 << 0


class LaserlockOut(IntEnum):
    usb_boot = 1
    david_end = 2
    rachel_end = 3
    light_off = 4
    cleanupLight = 5
    failedBootTrigger = 6
    bootupTrigger = 7
    rachel_end_announce = 8
    skip_to_seperation = 9      # in case the mother crashes lik in in HH


class LaserlockIn(IntEnum):
    david = 1
    rachel = 2
    seperationEnd = 3
    davidSeperated = 4     # status for T1
    rachelSeperated = 5    # status for T1
    timeout = 6


class LockerIn(IntEnum):
    serviceEnable = 1 << 4
    serviceDisable = 1 << 5


class LockerOut(IntEnum):
    serviceEnable = 1 << 3
    serviceDisable = 1 << 4
    selfDestruct = 24


labLight_trigger = "labLight"
ending_trigger = "upload"
lab_light_off = 1
lab_light_white = 2
lab_light_standby = 3
lab_light_on = 4
lab_rachel_end_announce = 5
lab_rachel_end = 6
lab_david_end_announce = 7

lab_dishout = 1 << 4
lab_dish1 = 32
lab_dish2 = 32 + 16
lab_dish3 = 64
lab_dish4 = 64 + 16
lab_dish5 = 64 + 32
lab_dish_rachel_end_announce = 64 + 32 + 16
lab_dish_david_end = 128
lab_dish_rachel_end = 128 + 16

# Begin, Video, Fumigation, SterilizationIntro, Sterilization, BioScanIntro, BioScan, BioScanDenied, Wrong, Opening

binary_pcfs = [airlock_in_pcf, laserlock_in_pcf]

blank_screen_pid = subprocess.Popen(["cvlc", "media/black_screen.jpg", "--no-embedded-video", "--fullscreen",
                                     "--no-video-title", "--video-wallpaper", "--quiet", "--loop"])


class States:
    def __init__(self):
        self.laserlock_door_armed = False
        self.laserlock_door_opened = False
        self.laserlock_fixed = False
        self.usb_booted = False
        self.truth_played = False
        self.upload_elancell = False
        self.upload_Rachel = False
        self.service = False
        self.stream_active = False


states = States()


def play_elancell_intro(*args):
    blank_screen_pid.kill()
    print("playing elancell intro")
    subprocess.Popen(['cvlc', "media/Welcome to Elancell_w_Audio.mp4",
                      "--no-embedded-video", "--fullscreen", '--no-video-title', '--video-on-top', '--quiet'])


def call_video(event_key, nw_sock):
    nw_sock.transmit(event_key)


class LaserLock:
    @staticmethod
    def arm_door(*args):
        states.laserlock_door_armed = True
        states.laserlock_door_opened = False

    @staticmethod
    def door_open_condition():
        if states.laserlock_door_armed and not states.laserlock_door_opened:
            states.laserlock_door_opened = True
            return True
        return False

    @staticmethod
    # @todo: just include set_fixed into condition
    def fixed_condition(*args):
        if not states.laserlock_fixed:
            states.laserlock_fixed = True
            return True
        return False

    @staticmethod
    def broken_conditions(*args):
        if states.laserlock_fixed:
            states.laserlock_fixed = False
            return True
        return False

    @staticmethod
    def play_truth_condition(*args):
        if not states.truth_played:
            states.truth_played = True
            states.stream_active = True
            return True
        return False

    @staticmethod
    def start_stream(*args):
        if not states.stream_active:
            states.stream_active = True
            return True
        return False


class USBScripts:
    @staticmethod
    def rachel_enabled_condition(*args):
        if not states.upload_Rachel and not states.upload_elancell:
            states.upload_Rachel = True
            return True
        return False

    @staticmethod
    def rachel_disabled_condition():
        if states.upload_Rachel:
            states.upload_Rachel = False
            return True
        return False

    @staticmethod
    def elancell_enabled_condition(*args):
        if not states.upload_elancell and not states.upload_Rachel:
            states.upload_elancell = True
            return True
        return False

    @staticmethod
    def elancell_disabled_condition(*args):
        if states.upload_elancell:
            states.upload_elancell = False
            return True
        return False

    @staticmethod
    def boot_condition(*args):
        usb_status = states.usb_booted
        if not usb_status:
            states.usb_booted = True
            return True
        else:
            return False

test_out = 0

event_map = {
    "test_0": {
        pcf_out_add: [test_out],
        pcf_out: [AirlockOut.test_0],
    },
    "test_1": {
        pcf_out_add: [test_out],
        pcf_out: [AirlockOut.test_1],
    },
    "test_2": {
        pcf_out_add: [test_out],
        pcf_out: [AirlockOut.test_2],
    },
    "test_3": {
        pcf_out_add: [test_out],
        pcf_out: [AirlockOut.test_3],
    },
    "test_4": {
        pcf_out_add: [test_out],
        pcf_out: [AirlockOut.test_4],
    },
    "test_5": {
        pcf_out_add: [test_out],
        pcf_out: [AirlockOut.test_5],
    },
    "test_6": {
        pcf_out_add: [test_out],
        pcf_out: [AirlockOut.test_6],
    },
    "test_7": {
        pcf_out_add: [test_out],
        pcf_out: [AirlockOut.test_7],
    }
}

# Only can be applied to non binary pinbased inputs
inverted_events = {}


def activate_sound(event_entry):
    print(event_entry)
    payload = dict(ip="192.168.178.172")

    try:
        sound_id_value = event_entry[sound_id]
        if event_entry.get(is_fx, True):
            payload["fx_id"] = sound_id_value
        else:
            payload["group_id"] = sound_id_value
    except KeyError:
        pass

    try:
        ret = requests.post("http://POD-ITX/AudioInterface.php", payload)
        print(f"send sound payload: {payload}")
        print(ret)
    except OSError as OSE:
        print(f"failed to request sound due to {OSE}")
        return







