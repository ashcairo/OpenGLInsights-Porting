LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libccjni

LOCAL_C_INCLUDES := \
				$(LOCAL_PATH)/ \
				$(LOCAL_PATH)/Rendering \
				$(LOCAL_PATH)/Tools \
\
				$(LOCAL_PATH)/../libzip \
\
				$(LOCAL_PATH)/../../../../Engine/Source \
				$(LOCAL_PATH)/../../../../Engine/Source/AI \
				$(LOCAL_PATH)/../../../../Engine/Source/Objects \
				$(LOCAL_PATH)/../../../../Engine/Source/Rendering \
				$(LOCAL_PATH)/../../../../Engine/Source/Scenes \
				$(LOCAL_PATH)/../../../../Engine/Source/Tools \
\
				$(LOCAL_PATH)/../../../../App/Source/ \

LOCAL_STATIC_LIBRARIES := libzip

# Make all warnings into errors
LOCAL_CPPFLAGS  := -Werror

# Reduce compilation time and make debugging produce the expected results.
# This is the default. So no need to turn on
#LOCAL_CPPFLAGS  += -O0

# CCRelease: Makes macros debugable
LOCAL_CPPFLAGS  += -g3

# CCRelease: We don't need accurate floating point maths, do we?
#LOCAL_CPPFLAGS  += -ffast-math
#LOCAL_CPPFLAGS  += -O3 -funroll-loops -DFPM_ARM

LOCAL_SRC_FILES := \
				main.cpp \
                CCJNI.cpp \
                CCPlatform.cpp \
\
                Rendering/CCDeviceRenderer.cpp \
                Rendering/CCTexturePNG.cpp \
\
                Tools/CCDeviceFileManager.cpp \
                Tools/CCDeviceURLManager.cpp \
\
                ../../../../Engine/Source/CCEngine.cpp \
                ../../../../Engine/Source/CCViewManager.cpp \
\
                ../../../../Engine/Source/AI/CCInterpolators.cpp \
\
                ../../../../Engine/Source/Objects/CCRenderable.cpp \
                ../../../../Engine/Source/Objects/CCSceneCollideable.cpp \
                ../../../../Engine/Source/Objects/CCSceneObject.cpp \
                ../../../../Engine/Source/Objects/CCTile3D.cpp \
                ../../../../Engine/Source/Objects/CCTile3DButton.cpp \
\
                ../../../../Engine/Source/Rendering/CCMatrix.cpp \
                ../../../../Engine/Source/Rendering/CCModelBase.cpp \
                ../../../../Engine/Source/Rendering/CCPrimitiveBase.cpp \
                ../../../../Engine/Source/Rendering/CCPrimitiveSquare.cpp \
                ../../../../Engine/Source/Rendering/CCRenderer.cpp \
                ../../../../Engine/Source/Rendering/CCRenderTools.cpp \
                ../../../../Engine/Source/Rendering/CCTextureBase.cpp \
                ../../../../Engine/Source/Rendering/CCTextureManager.cpp \
\
                ../../../../Engine/Source/Scenes/CCSceneAppUI.cpp \
                ../../../../Engine/Source/Scenes/CCSceneBase.cpp \
\
                ../../../../Engine/Source/Tools/CCBaseTools.cpp \
                ../../../../Engine/Source/Tools/CCBaseTypes.cpp \
                ../../../../Engine/Source/Tools/CCCallbacks.cpp \
                ../../../../Engine/Source/Tools/CCCollisionTools.cpp \
                ../../../../Engine/Source/Tools/CCFileManager.cpp \
                ../../../../Engine/Source/Tools/CCMathTools.cpp \
                ../../../../Engine/Source/Tools/CCOctree.cpp \
                ../../../../Engine/Source/Tools/CCURLManager.cpp \
                ../../../../Engine/Source/Tools/CCVectors.cpp \
\
                ../../../../App/Source/CCAppEngine.cpp \
                ../../../../App/Source/SceneSample.cpp \


LOCAL_LDLIBS  := -llog -lGLESv2
LOCAL_LDLIBS  += -L$(SYSROOT)/usr/lib -ldl
LOCAL_LDLIBS  += -L$(SYSROOT)/usr/lib -lz

include $(BUILD_SHARED_LIBRARY)
