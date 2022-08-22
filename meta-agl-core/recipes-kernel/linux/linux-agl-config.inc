# Common include file that defines AGL's variables for kernel
# configuration.
#
# Can be included directly for kernel.bbclass based recipes that
# use their own config fragment merging scheme, or use the
# KERNEL_CONFIG_FRAGMENTS variable (e.g. meta-ti, meta-qcom).
#
# Fragments should be added to AGL_KCONFIG_FRAGMENTS as just the
# .cfg filename with +=.  Appending to AGL_KCONFIG_FRAGMENTS (i.e.
# using _append) should only be done for:
# - Specific AGL features that set an override in their feature
#   template, e.g. agl-netboot.
# - To add essential configuration for core target machines like
#   qemu.  Note that appending fragments directly to SRC_URI
#   would work for qemu targets since they use linux-yocto, but
#   the AGL_KCONFIG_FRAGMENTS mechanism is recommended for
#   consistency and to preserve the option of disabling all AGL
#   additions by overriding AGL_KERNEL_SRC.
#
# In general, care shoud be taken to preserve the ability to set
# AGL_KCONFIG_FRAGMENTS to "" to disable non-essential (from a build
# perspective) AGL additions.
#

FILESEXTRAPATHS:prepend := "${THISDIR}/linux:"

# NOTE: AGL_KERNEL_SRC is explicitly intended as an intermediate variable
#       that can be used as a last resort to completely disable all AGL
#       additions, modifying or appending it is not recommended.
AGL_KERNEL_SRC ?= "${@' '.join(['file://' + x for x in d.getVar('AGL_KCONFIG_FRAGMENTS').split()])}"

SRC_URI:append = " ${AGL_KERNEL_SRC}"

# For meta-ti and meta-qcom
KERNEL_CONFIG_FRAGMENTS ?= "${@' '.join(['${WORKDIR}/' + x for x in d.getVar('AGL_KCONFIG_FRAGMENTS').split()])}"

# Extra configuration options for the AGL kernel
AGL_KCONFIG_FRAGMENTS += " \
    can-bus.cfg \
    fanotify.cfg \
    overlayfs.cfg \
    audit.cfg \
    ${@bb.utils.contains('DISTRO_FEATURES', 'selinux', 'selinux.cfg', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'systemd-required.cfg', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'systemd-sandbox.cfg', '', d)} \
    lttng.cfg \
"

AGL_KCONFIG_FRAGMENTS += " ${@bb.utils.contains('AGL_XEN_GUEST_WANTED','1','xen_domu.cfg','',d)}"

# Base ALSA support and other sound related configuration
AGL_KCONFIG_FRAGMENTS += "sound.cfg"

# Enable support for SystemTap
AGL_KCONFIG_FRAGMENTS += "${@bb.utils.contains('DISTRO_FEATURES', 'agl-devel', 'systemtap.cfg', '', d)}"

#
# Feature override and qemu specific appends:
#

# Enable required features for the agl-netboot feature
AGL_KCONFIG_FRAGMENTS:append:netboot = " \
    nbd.cfg \
    ramdisk.cfg \
"

# Add hda audio and required virtio device support for qemu
AGL_KCONFIG_FRAGMENTS:append:qemuall = " \
    sound-hda.cfg \
    qemu-virtio.cfg \
    qemu-drm.cfg \
"

AGL_KCONFIG_FRAGMENTS:append:virtio-all = " \
    sound-hda.cfg \
"

# Configuration for using the ARM virt machine (and not versatilepb)
AGL_KCONFIG_FRAGMENTS:append:qemuarm = " qemuarm.cfg"

# Additional drivers for virtual machines
# OVERRIDES save us some c'n'p below ...
OVERRIDES:prepend:qemux86 = "virtualmachine:"
OVERRIDES:prepend:qemux86-64 = "virtualmachine:"
AGL_KCONFIG_FRAGMENTS:append:virtualmachine = " vbox-vmware-sata.cfg"

# Extra configuration for using qemux86-64 image on physical hardware
AGL_KCONFIG_FRAGMENTS:append:qemux86-64 = " \
    x86-extra-graphic-devices.cfg \
    x86-net-devices.cfg \
    x86-security-tpm.cfg \
    x86-usb-devices.cfg \
    x86-upsquare.cfg \
"
