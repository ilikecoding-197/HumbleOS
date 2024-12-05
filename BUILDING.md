# Building
# Prerequisites
Before you build HumbleOS, you need to install some **prerequisites**. To do that, run the command below for your distro.

| **Distribution**           | **Command**                                      |
|----------------------------|--------------------------------------------------|
| **Debian/Ubuntu**          | `sudo apt-get install nasm gcc xorriso grub-pc`  |
| **Red Hat/CentOS/Fedora**  | `sudo yum install nasm gcc xorriso grub2`        |
| **Fedora 22+, CentOS 8+, RHEL 8+** | `sudo dnf install nasm gcc xorriso grub2`   |
| **Arch Linux/Manjaro**     | `sudo pacman -S nasm gcc xorriso grub`           |

# Build!
Now just build by simply running `bash build.sh`.

# Running
To run, make sure you have QEMU and start it with the ISo file `build/os.iso`.
