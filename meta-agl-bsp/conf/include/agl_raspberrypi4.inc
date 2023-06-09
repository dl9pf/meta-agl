# Maximum memory allowed to be assigned
GPU_MEM = "256"

# Add CMA to the kernel arguments for SOTA
OSTREE_KERNEL_ARGS:sota:append = " cma=256M"

# use u-boot always
RPI_USE_U_BOOT = "1"

# Use UART for serial console
ENABLE_UART ?= "1"

# For libomxil
#LICENSE_FLAGS_ACCEPTED += "commercial"

# For linux-firmware-rpidistro
LICENSE_FLAGS_ACCEPTED += "synaptics-killswitch"

IMAGE_INSTALL:append = " kernel-modules linux-firmware-rpidistro-bcm43430 linux-firmware-rpidistro-bcm43455 create-combined-dtb "

# Psplash causes crash on first boot on RPi
IMAGE_FEATURES:remove = "splash"

# Build updatable image. Only takes effect when sota.bbclass is inherited
DISTRO_FEATURES:append = " sota"

# Add xen build, if the xen feature activated
DISTRO_FEATURES:append = " ${@bb.utils.contains('AGL_XEN_WANTED', '1', ' xen', '', d)}"

# add xen-tools to build
IMAGE_INSTALL:append = " ${@bb.utils.contains('AGL_XEN_WANTED', '1', 'xen-tools xen-mkimage-rpi4', '', d)}"

# Add xen-raspberry binary to boot files
IMAGE_BOOT_FILES:append = " ${@bb.utils.contains('AGL_XEN_WANTED','1', 'xen-${MACHINE}', '',d)}"

#DISPLAY CONFIGURATION
WESTON_DISPLAYS:raspberrypi4 = " hdmi-a-1-90 hdmi-a-2-90"
