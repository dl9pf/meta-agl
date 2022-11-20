BBLAYERS =+ " ${METADIR}/meta-agl/meta-netboot \
            "

# Indirection to avoid duplicate inclusions of the same folder into BBLAYERS
# the evaluation is in the bblayers.conf.sample in meta-agl/meta-agl-core/conf/templates/base
AGL_META_PYTHON = "${METADIR}/external/meta-openembedded/meta-python"
AGL_META_NETWORKING = "${METADIR}/external/meta-openembedded/meta-networking"
