require recipes-kernel/linux/linux-agl.inc

FILESEXTRAPATHS:prepend := "${THISDIR}/${BPN}:"

# enable AMDgpu 
AGL_KCONFIG_FRAGMENTS += "radeon.cfg"
