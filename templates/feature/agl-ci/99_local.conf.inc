AGL_FEATURES:append = " AGLCI"
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

BB_HASHSERVE = "${AGL_HOST_HASHSERV}:8383"
BB_SIGNATURE_HANDLER = "OEEquivHash"

PRSERV_HOST = "${AGL_HOST_PRSERV}:8181"

IMAGE_INSTALL:append = " curl"

# Workaround for CI builds since YP kirkstone v4.0.11
# until uninative is fixed or builders are uprev'd.
UNINATIVE_MAXGLIBCVERSION = "2.37"
UNINATIVE_VERSION:forcevariable = "4.1"
UNINATIVE_URL = "http://downloads.yoctoproject.org/releases/uninative/${UNINATIVE_VERSION}/"
UNINATIVE_CHECKSUM[aarch64] = "b6ff9171aa7d3828bc81197822e804725908856bbd488bf412121cc0deddcb60"
UNINATIVE_CHECKSUM[i686] = "6354fd2e09af1f111bad5e34ce7af4f9ad7cd266188af7eeceaeb982afd5354b"
UNINATIVE_CHECKSUM[x86_64] = "f83eca543170adfd2432b135ca655922a4303622d73cc4b13e92b973cdf49e3a"
