DISTRO_FEATURES:append = " AGLCI"
# opencv seems to have a parallel make bug
# ...contrib/modules/xfeatures2d/test/test_features2d.cpp:51:10: fatal error: features2d/test/test_detectors_regression.impl.hpp: No such file or directory
PARALLEL_MAKE:pn-opencv = "-j 1"

AGL_HOST_PREMIRROR ?= "http://download.automotivelinux.org"
AGL_HOST_SSTATE_MIRROR ?= "http://download.automotivelinux.org"

PREMIRRORS = " \
git://.*/.*   ${AGL_HOST_PREMIRROR}/AGL/mirror/  \n \
ftp://.*/.*   ${AGL_HOST_PREMIRROR}/AGL/mirror/  \n \
http://.*/.*  ${AGL_HOST_PREMIRROR}/AGL/mirror/  \n \
https://.*/.* ${AGL_HOST_PREMIRROR}/AGL/mirror/  \n \
"

SSTATE_MIRRORS = "  file://.* ${AGL_HOST_SSTATE_MIRROR}/sstate-mirror/master/${DEFAULTTUNE}/PATH;downloadfilename=PATH \n                  "

#INHERIT += "packagefeed-stability"
INHERIT += "buildhistory"
INHERIT += "buildstats"
INHERIT += "buildstats-summary"

# setup for PRSERV and HASHSERV
AGL_HOST_PRSERV ?= "10.30.72.18"
AGL_HOST_HASHSERV ?= "10.30.72.18"

BB_HASHSERVE = "${AGL_HOST_HASHSERV}:8686"
BB_SIGNATURE_HANDLER = "OEEquivHash"

PRSERV_HOST = "${AGL_HOST_PRSERV}:8585"


# Workaround for CI builds since YP dunfell v3.1.27
# until uninative is fixed or builders are uprev'd.
UNINATIVE_VERSION:forcevariable = "3.9"
UNINATIVE_CHECKSUM[aarch64] = "de35708c95c34573af140da910132c3291ba4fd26ebf7b74b755ada432cdf07b"
UNINATIVE_CHECKSUM[i686] = "adac07b08adb88eb26fc7fd87fee0cec9d5be167bf7c5ffd3a549a2a6699c29c"
UNINATIVE_CHECKSUM[x86_64] = "3dd82c3fbdb59e87bf091c3eef555a05fae528eeda3083828f76cd4deaceca8b"
