SUMMARY = "Provides a set of tools for development for AGL DISTRO"
LICENSE = "MIT"

inherit packagegroup

RDEPENDS:${PN} = "\
        strace \
        ldd \
        less \
        vim \
        lsof \
        gdb \
        screen \
        usbutils \
        rsync \
        pstree \
        procps \
        libxslt-bin \
        pciutils \
        openssh-sftp-server \
        zstd \
"

