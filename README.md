# uarch-attacks-mcus

Welcome! This repository hosts the code, datasets, and tools developed for my PhD thesis investigating software-based microarchitectural attacks targeting low-end IoT embedded systems.

## Overview

While software-based microarchitectural attacks have been extensively studied on high-end processors, the MCU-powered embedded devices have been largely overlooked.
This thesis bridges this gap by exploring microarchitectural attacks on MCUs.

## Repository Structure

| Directory                 | Description |
|-----------                |-------------|
| Chapter-4-First-Evidence  | Artifacts of Chapter 4 - Arm Cortex-M Microarchitectural Analysis.  |
| Chapter-5-BUSted          | Artifacts of Chapter 5 - BUSted: Non-Convention Microarchitectural Leakage on MCUs. |
| Chapter-6-M-Step          | Artifacts of Chapter 6 - M-Step: Single-Step Framework for TrustZone-M.  |
| Chapter-7-Breaking-Crypto | Artifacts of Chapter 7 - Breaking Cryptographic Applications on MCUs.  |

---

## Prerequisites

### 1. Install Nix Package Manager

The development environment uses Nix for reproducible builds. Install Nix and add your user to the `nix-users` group:

```bash
sh <(curl --proto '=https' --tlsv1.2 -L https://nixos.org/nix/install) --no-daemon
```

Enable experimental features by adding to `~/.config/nix/nix.conf` or `/etc/nix/nix.conf`:

```
experimental-features = nix-command flakes
```

### 2. Install STM32 Programmer

Download and install [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html) (requires ST account).

Verify installation:
```bash
STM32_Programmer_CLI --version
```

Ensure `STM32_Programmer_CLI` is in your `PATH`.

### 3. Configure USB Permissions (udev Rules)

Create `/etc/udev/rules.d/99-stlink.rules` with the following content:

```
# ST-LINK V3 (STM32L5 Discovery / Nucleo)
SUBSYSTEMS=="usb", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="374e", MODE="0666", GROUP="plugdev"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="374b", MODE="0666", GROUP="plugdev"
```

Reload udev rules:
```bash
sudo udevadm control --reload-rules
sudo udevadm trigger
```

> **Note**: Without these rules, `sudo` is required for every deployment.

---

## Getting Started

### 1. Clone the repository:

```bash
git clone https://github.com/ESCristiano/uarch-attacks-mcus.git
cd uarch-attacks-mcus
git submodule update --init --recursive
```

### 2. Enter the Development Environment:

```bash
nix develop
```

This provides a reproducible shell with all required dependencies (ARM toolchain, CMake, Python packages, etc.).
Note: Run nix develop in every new terminal session.

### 3. Run the experiments:

Each sub-directory provides a script to reproduce the experiments/PoCs for each artifact of my PhD Thesis. 

## Citation

If you use this work in your research, please cite the corresponding thesis or publication (TBD!!).
