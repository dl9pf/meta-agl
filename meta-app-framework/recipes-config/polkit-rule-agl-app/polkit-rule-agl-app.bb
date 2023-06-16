SUMMARY = "Rule for agl-driver to control agl-app@ services"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://50-agl-app.rules"

# need to include polkit group rules
include recipes-extended/polkit/polkit-group-rule.inc
USERADD_PACKAGES = "${PN}"

do_install() {
    install -m 700 -d ${D}${sysconfdir}/polkit-1/rules.d
    chown polkitd:root ${D}${sysconfdir}/polkit-1/rules.d
    install -m 0644 ${WORKDIR}/50-agl-app.rules ${D}${sysconfdir}/polkit-1/rules.d
}

FILES:${PN} += "${sysconfdir}/polkit-1/rules.d/*.rules"
