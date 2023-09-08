require ${@bb.utils.contains('AGL_FEATURES', 'aglcore', 'gstreamer1.0-plugins-bad_aglcore.inc', '', d)}
