# This is the original place, but we need to tweak layer.conf
BBLAYERS =+ "${METADIR}/bsp/meta-sifive"

# For now, tweak the layer's compatibility to allow using with kirkstone,
# since we know it works without major issues.
LAYERSERIES_COMPAT_meta-sifive:append = " kirkstone"
