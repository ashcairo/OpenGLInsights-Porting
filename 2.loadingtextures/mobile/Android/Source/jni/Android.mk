LOCAL_PATH := $(call my-dir)

$(shell scripts/copy_textures.sh)

include $(call all-subdir-makefiles)
