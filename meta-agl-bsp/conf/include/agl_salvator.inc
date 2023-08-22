require conf/include/agl_rcar.inc

IMAGE_INSTALL:append:rcar-gen3 = " \
    kernel-module-vspm-if \
"

