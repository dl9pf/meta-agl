## Introduction

The `meta-agl` layer provides the minimal set of software
to boot an AGL Distribution system.
You use this layer as the minimal core on which to build AGL profiles.

**NOTE:** The `meta-agl` layer does not include a reference UI.

## Sub-Layers

The `meta-agl` layer itself contains many sub-layers and files.
Following is a "tree" look at the layer:

```
.
├── LICENSE
├── LICENSE.GPL-2.0-only
├── README-AGL.md
├── README.md -> meta-agl.md
├── agl-layers-overview.md
├── docs
├── LICENSE.MIT
├── meta-agl-bsp
├── meta-agl-core
├── meta-agl-drm-lease
├── meta-agl-flutter
├── meta-agl-ic
├── meta-agl-ivi
├── meta-agl-kuksa-val
├── meta-agl.md
├── meta-app-framework
├── meta-netboot
├── meta-pipewire
├── scripts
└── templates
```

This list provides some overview information on the files and sub-layers
in `meta-agl`:

* LICENSE* : licenses of the various components.
* README* : RTFM
* agl-layers-overview.md: references to the various layers that make up AGL
* `docs`: Contains files that support AGL documentation.
* `meta-agl-bsp`: Contains adaptations for recipes and required packages
  to boot an AGL distribution on targeted hardware and emulation (i.e. QEMU).
* `meta-agl-core`: This is the core layer with essential recipes and the distro.
* `meta-agl-drm-lease`: The recipes for supporting display isolating feature (DRM master sharing).
* `meta-agl-flutter`: The recipes for AGL Flutter support.
* `meta-agl-ic`: The recipe set for Instrument Cluster platform.
* `meta-agl-ivi`: The recipe set for IVI platform.
* `meta-agl-kuksa-val`: The recipes for AGL Vehicle API support using kuksa.
* `meta-app-framework`: The recipes for AGL IVI application framework.
* `meta-netboot`: Contains recipes and configuration adjustments to allow network
  boot through network block device (NBD) since network file system (NFS) does not
  support security labels.
* `meta-pipewire`: Configuration and recipes supporting pipewire as audio manager
* `scripts`: AGL development setup and support scripts.
* `templates`: Base, feature, and machine templates used in the AGL development
  environment.

## Packagegroups

This section describes the AGL
[packagegroup](https://docs.yoctoproject.org/dev-manual/common-tasks.html#customizing-images-using-custom-package-groups)
design:

### core non-ui packagegroups
* meta-agl-core/recipes-platform/packagegroups/packagegroup-agl-core-connectivity.bb
* meta-agl-core/recipes-platform/packagegroups/packagegroup-agl-core-devel.bb
* meta-agl-core/recipes-platform/packagegroups/packagegroup-agl-core-security.bb
* meta-agl-core/recipes-platform/packagegroups/packagegroup-agl-core-os-commonlibs.bb

### graphical subsystem
* meta-agl-core/recipes-platform/packagegroups/packagegroup-agl-profile-graphical.bb
* meta-agl-core/recipes-platform/packagegroups/packagegroup-agl-graphical-multimedia.bb
* meta-agl-core/recipes-platform/packagegroups/packagegroup-agl-graphical-weston.bb

### image-related packagegroups
* meta-agl-core/recipes-platform/packagegroups/packagegroup-agl-core-boot.bb
* meta-agl-core/recipes-platform/packagegroups/packagegroup-agl-image-boot.bb
* meta-agl-core/recipes-platform/packagegroups/packagegroup-agl-image-minimal.bb

### QA/Test related packagegroups
* meta-agl-core-test/recipes-test/packagegroups
* meta-agl-core-test/recipes-test/packagegroups/packagegroup-agl-test.bb

## Images

### (Barely) bootable image
* meta-agl-core/recipes-platform/images/agl-image-boot.bb

### minimal image and SDK (console)
* meta-agl-core/recipes-platform/images/agl-image-minimal.bb
* meta-agl-core/recipes-platform/images/agl-image-minimal-crosssdk.bb

### weston-based image for re-use
* meta-agl-core/recipes-platform/images/agl-image-weston.bb

### image for use in the YP autobuilder
* meta-agl-core/recipes-platform/images/agl-image-core-autobuilder.bb

### image with extra QA tooling (e.g. to run LTP)
* meta-agl-core-test/images/agl-image-minimal-qa.bb



**Note:** For update details on this page please refer

<https://git.automotivelinux.org/AGL/meta-agl/tree/meta-agl.md>

<!-- CI test: fork pull request routing to the untrusted runner. Do not merge. -->
