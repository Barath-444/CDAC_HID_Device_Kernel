# CDAC HID Device Kernel Monitoring System

## Overview
This project was developed during an **internship at CDAC Chennai** specifically for enhancing the security of **digital exam centers**. 

The system consists of several Linux kernel modules and a user-space utility designed to monitor and respond to unauthorized hardware connections (USB, HDMI, Bluetooth, Ethernet) that could be used for cheating or information leaks during exams.

## Key Features
- **USB HID Detection**: Monitors unauthorized keyboards, mice, and other input devices.
- **HDMI/VGA Detection**: Detects external monitor connections via framebuffer notifications.
- **Automated Response**: Initiates a system shutdown if multiple monitors or unauthorized devices are detected.
- **Bluetooth Logging**: Captures MAC addresses of all Bluetooth ACL connections using kernel `kprobes`.
- **Ethernet Tracking**: Logs the activation of wired network interfaces.
- **Secure Communication**: Implements a `procfs` bridge between user-space monitoring and kernel-level logging.

---

## Technical Architecture

### 1. HDMI and USB HID Detection (`exam_alert.c`)
Monitors USB and HDMI connections. If two or more devices are detected (e.g., a secondary monitor or an extra keyboard), it triggers a controlled system shutdown.
- **APIs Used**: `usb_register_notify`, `fb_register_client`, `call_usermodehelper`.
- **Logging**: Timestamped entries with Vendor/Product IDs in IST (+5:30).

### 2. Bluetooth ACL Connection Logger (`bt_kprobe.c`)
Uses `kprobes` to intercept the `hci_connect_acl` function.
- **Logic**: Extracts the destination MAC address, checks for duplicates in a static array, and logs connection events to `dmesg`.

### 3. Ethernet Interface Logger (`eth_alert.c`)
Listens for `NETDEV_UP` events specifically for Ethernet interfaces (`ARPHRD_ETHER`).
- **APIs Used**: `register_netdevice_notifier`.

### 4. Monitor Detection Bridge (`monitor_detector.c` & `user_monitor_watcher.c`)
- **Kernel Side**: Creates `/proc/monitor_status` to receive status updates.
- **User Space**: A poller that scans `/sys/class/drm/` every 2 seconds for external monitor changes and reports them back to the kernel.

---

## Project Structure
- `main/`: Integrated version of the security module.
- `bt_kprobe_module/`: Standalone Bluetooth kprobe tracker.
- `eth_alert_module/`: Standalone Ethernet status monitor.
- `hid_alert_module/`: Standalone HID device monitor.
- `monitor_in_test/`: Contains the `procfs` detector and user-space monitor watcher.

---

## Installation & Testing
### Prerequisites
- Linux Kernel Headers
- Build-essential (gcc, make)

### Building the Modules
Each subdirectory contains a `Makefile`. To build a module:
```bash
cd <module_directory>
make
```

### Loading the Module
```bash
sudo insmod <module_name>.ko
```

### Viewing Logs
```bash
dmesg | grep "EXAM ALERT"
```

---

## Credits
This project is an official output of an internship at **CDAC Chennai (Centre for Development of Advanced Computing)**. 
**Objective**: To secure digital exam centers against hardware-based tampering.

---

## Future Work
- Heuristic-based splitter detection using EDID analysis.
- Live desktop alert interface.
- Centralized dashboard for exporting kernel logs across multiple exam terminals.
