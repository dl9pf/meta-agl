require conf/include/agl_salvator-nogfx.inc
SOC_FAMILY = "r8a7795"
BOARD_NAME = "h3-salvator-x"

DTB_SUFFIX = "r8a77951-salvator-xs"

# do use software rendering to display the compositor
# used for images w/o binary-only driver
WESTON_USE_PIXMAN = "1"
