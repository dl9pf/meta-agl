require recipes-kernel/linux/linux-agl.inc

FILESEXTRAPATHS_prepend := "${THISDIR}/${BPN}:"

AGL_KCONFIG_FRAGMENTS += "cma-256.cfg"
