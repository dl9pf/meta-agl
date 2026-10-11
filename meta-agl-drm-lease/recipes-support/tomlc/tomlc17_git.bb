SUMMARY = "A lightweight, strictly compliant TOML v1.1 parser for C and C++."
HOMEPAGE = "https://github.com/cktan/tomlc17"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b39f4d8d02338aa382a2dde5592173ef"

SRC_URI = "git://github.com/cktan/tomlc17;protocol=https;branch=main \
           file://0001-Avoid-build-error-that-is-caused-by-compile-options.patch"

SRCREV = "fba134d7d1f9b58414f908600b2869f7e55ff2c4"
PV = "R261003+git${SRCREV}"

inherit pkgconfig

do_compile() {
    oe_runmake
}

do_install() {
    install -d ${D}${includedir}
    install -m 0644 ${S}/src/tomlc17.h ${D}${includedir}/
    install -m 0644 ${S}/src/tomlcpp.hpp ${D}${includedir}/

    install -d ${D}${libdir}
    install -m 0644 ${S}/src/libtomlc17.a ${D}${libdir}/
    install -m 0644 ${S}/src/libtomlc17.so.1.0 ${D}${libdir}/
    ln -s ./libtomlc17.so.1.0 ${D}${libdir}/libtomlc17.so.1
    ln -s ./libtomlc17.so.1 ${D}${libdir}/libtomlc17.so

    install -d ${D}${libdir}/pkgconfig
    cat << EOF > ${D}${libdir}/pkgconfig/libtomlc17.pc
prefix=${prefix}
includedir=${includedir}
libdir=${libdir}

Name: libtomlc17
Description: TOML parser in C (libtomlc17)
Version: 1.0
Libs: -L\${libdir} -ltomlc17
Cflags: -I\${includedir}
EOF

}

FILES:${PN}-dev = " \
    ${includedir}/tomlc17.h ${includedir}/tomlcpp.hpp \
    ${libdir}/pkgconfig/libtomlc17.pc ${libdir}/libtomlc17.a \
    ${libdir}/libtomlc17.so \
    "
