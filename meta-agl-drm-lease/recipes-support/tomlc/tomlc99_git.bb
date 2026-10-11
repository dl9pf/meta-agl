SUMMARY = "A lightweight TOML v1.0 parser for C."
HOMEPAGE = "https://github.com/agl-ic-eg/tomlc99"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=606ec2ecca9fcf1170f71df9104a7d29"

SRC_URI = "git://github.com/agl-ic-eg/tomlc99;protocol=https;branch=main"

PV = "1.1"
SRCREV = "535a7051d999ef6856ce57407f6f97660009a7f1"

TARGET_CC_ARCH += "${LDFLAGS}"

do_install () {
        oe_runmake install prefix=${D}/${prefix}

        ln -rs ${D}/${prefix}/lib/libtoml.so.0.0 ${D}${prefix}/lib/libtoml.so

        sed 's:^prefix=.*:prefix=${prefix}:' ${S}/libtoml.pc.sample > libtoml.pc
        install -d ${D}${prefix}/lib/pkgconfig
        install libtoml.pc ${D}${prefix}/lib/pkgconfig/
}
