SOC_FAMILY = "r8a779f0"
BOARD_NAME = "s4sk"

#ws2.0
DTB_SUFFIX = "r8a779f0-s4sk"
WKS_FILE = "singlepart-noloader.wks"
IMAGE_INSTALL:append = " \
    kernel-devicetree \
"
