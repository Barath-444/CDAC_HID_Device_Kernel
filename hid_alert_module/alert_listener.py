#!/usr/bin/env python3
import os
import pyudev
import subprocess

# Ensure GUI environment variables are set
os.environ["DISPLAY"] = ":0"
os.environ["DBUS_SESSION_BUS_ADDRESS"] = "unix:path=/run/user/1000/bus"
os.environ["XDG_RUNTIME_DIR"] = "/run/user/1000"

context = pyudev.Context()
monitor = pyudev.Monitor.from_netlink(context)
monitor.start()

print("[+] Listening for HID uevents...")

for device in iter(monitor.poll, None):
    props = dict(device.properties)
    print("[UEVENT]", props)  # Debug output

    # Match custom kernel uevent field
    if props.get("EVENT_TYPE") == "HID_ALERT":
        print("[!] HID Alert received!")

        # Show a reliable popup using zenity (GUI dialog)
        subprocess.run([
            "zenity",
            "--info",
            "--title=⚠️ HID ALERT",
            "--text=s0s team test : danger HID device detected"
        ])
