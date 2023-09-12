SUMMARY = "Reference Wayland compositor for AGL"
DESCRIPTION = "The AGL compositor is a reference Wayland server for Automotive \
Grade Linux, using libweston as a base to provide a graphical environment for \
the automotive environment."

HOMEPAGE = "https://gerrit.automotivelinux.org/gerrit/q/project:src%252Fagl-compositor"
SECTION = "x11"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://COPYING;md5=fac6abe0003c4d142ff8fa1f18316df0"

DEPENDS = "wayland wayland-protocols wayland-native weston"

SRC_URI = "git://gerrit.automotivelinux.org/gerrit/src/agl-compositor.git;protocol=https;branch=${AGL_BRANCH}"
SRCREV = "a68408367cf5e254d981e1d43ba261b9aade6166"
AGL_BRANCH:aglnext = "next"
SRCREV:aglnext = "${AUTOREV}"

PV = "0.0.10+git${SRCPV}"
S = "${WORKDIR}/git"

PACKAGECONFIG ?= ""
PACKAGECONFIG[policy-rba] = "-Dpolicy-default=rba,,librba,librba rba-config"
PACKAGECONFIG[policy-deny-all] = "-Dpolicy-default=deny-all,,"
PACKAGECONFIG[grpc-proxy] = "-Dgrpc-proxy=true,-Dgrpc-proxy=false,grpc grpc-native,grpc agl-shell-grpc-server"

inherit meson pkgconfig python3native

# Reuse include file from upstream weston since we have the same requirements
require recipes-graphics/wayland/required-distro-features.inc

PACKAGES =+ "agl-shell-grpc-server"

LDFLAGS:append:riscv64 = " -Wl,--no-as-needed -latomic -Wl,--as-needed"

FILES:${PN} = " \
    ${bindir}/agl-compositor \
    ${bindir}/agl-screenshooter \
    ${libdir}/agl-compositor/libexec_compositor.so.0 \
    ${libdir}/agl-compositor/libexec_compositor.so.0.0.21 \
"

FILES:agl-shell-grpc-server = " \
    ${libdir}/agl-compositor/agl-shell-grpc-server \
"

RDEPENDS:${PN} += " \
    agl-compositor-init \
    xkeyboard-config \
"

FILES:${PN}-dev += " \
    ${datadir}/agl-compositor/protocols/agl-shell.xml \
    ${datadir}/agl-compositor/protocols/agl-shell-desktop.xml \
    ${libdir}/agl-compositor/libexec_compositor.so \
"
