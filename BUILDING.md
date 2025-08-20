# Building

## Prerequisites
Before you build HumbleOS, you need to install some **prerequisites**. To do that, run the command below for your distro.

| **Distribution**                   | **Command**                                    |
|------------------------------------|------------------------------------------------|
| **Debian/Ubuntu**                  | `sudo apt-get install nasm xorriso grub-pc`    |
| **Red Hat/CentOS/Fedora**          | `sudo yum install nasm xorriso grub2`          |
| **Fedora 22+, CentOS 8+, RHEL 8+** | `sudo dnf install nasm xorriso grub2`      |
| **Arch Linux/Manjaro**             | `sudo pacman -S nasm xorriso grub`             |\

You might, if youre using the config system, need to install `npyscreen`:

`pip install npyscreen`

## Cross-compilier
Before you try to build HumbleOS, you need a cross-compiler. Really, just follow [the OSDev tutorial](https://wiki.osdev.org/GCC_Cross-Compiler), to where your cross compiler and tools are at `~/opt/cross/bin/i686-elf-*`, and post a issue if you have any issues.

## Configuration
Start by creating the default configuration with `make defconfog`.
## Build!
Now just build by simply running `make`.

## Running
To run, make sure you have QEMU and start it with the ISO file `build/os.iso`.

## Cleaning build files
To clean, just run `make clean`.