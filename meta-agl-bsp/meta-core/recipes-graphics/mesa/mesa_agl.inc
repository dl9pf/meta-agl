SRC_URI += "file://0001-virgl-don-t-a-use-staging-when-a-resources-created-w.patch"

# The gallium-llvm is recommended as software 3D graphics renderer
GALLIUM_LLVM = "gallium-llvm"
PACKAGECONFIG:append:qemux86 = " gallium ${GALLIUM_LLVM}"
PACKAGECONFIG:append:qemux86-64 = " gallium ${GALLIUM_LLVM}"
PACKAGECONFIG:append:intel-corei7-64 = " gallium ${GALLIUM_LLVM}"

DRIDRIVERS:append:intel-corei7-64 = ",i965"
