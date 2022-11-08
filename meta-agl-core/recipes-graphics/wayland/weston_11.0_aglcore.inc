FILESEXTRAPATHS:append := ":${THISDIR}/weston"

PACKAGECONFIG:append = "${@bb.utils.contains('DISTRO_FEATURES', 'weston-remoting', ' remoting', '', d)}"

SRC_URI:append = "file://0001-libweston-Send-name-description-update-wl_output-to-.patch \
		  file://0001-simple-touch-Add-maximized-fullscreen-states.patch \
		  file://0001-libweston-weston-log-Add-a-iterator-helper-for-debug.patch "

# Split weston-terminal out of the main package to allow installing it
# separately for use with other compositors.  The icons, etc. that end
# up in /usr/share/weston are also split out into a separate package
# since they are required for weston-terminal and some of the programs
# packaged in weston-examples in addition to ivi-shell in the main
# package.

PACKAGE_BEFORE_PN += "${PN}-data ${PN}-terminal"

FILES:${PN}-data = "${datadir}/weston"

FILES:${PN}-terminal = "${bindir}/weston-terminal"
RDEPENDS:${PN}-terminal += "weston-data liberation-fonts"

RDEPENDS:${PN}-examples += "weston-data"

FILES:${PN}:remove = "weston-terminal"
RDEPENDS:${PN}:remove = "liberation-fonts"
RDEPENDS:${PN} += "weston-data weston-terminal"
