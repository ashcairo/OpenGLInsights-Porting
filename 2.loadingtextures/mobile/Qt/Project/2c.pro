# CCRELEASE: Remove for release
debug:DEFINES += DEBUGON

QT              += opengl network phonon
TARGET 		 = 2c
TEMPLATE 	 = app

OTHER_FILES += \
    ../../Engine/* \
    ../../Engine/Shaders/basic.fx \
    ../../Engine/Shaders/phong.fx

SOURCES 	+=  \
                ../Source/main.cpp \
                ../Source/CCGLView.cpp \
                ../Source/CCMainWindow.cpp \
                ../Source/CCVideoView.cpp \
                ../Source/CCPlatform.cpp \
                ../Source/CCRenderThread.cpp \
                ../Source/Rendering/CCDeviceRenderer.cpp \
                ../Source/Rendering/CCTexturePNG.cpp \
                ../Source/Tools/CCDeviceFileManager.cpp \
                ../Source/Tools/CCDeviceURLManager.cpp \
\
\
                ../../Engine/Source/CCEngine.cpp \
                ../../Engine/Source/CCViewManager.cpp \
\
                ../../Engine/Source/AI/CCInterpolators.cpp \
\
                ../../Engine/Source/Objects/CCRenderable.cpp \
                ../../Engine/Source/Objects/CCSceneCollideable.cpp \
                ../../Engine/Source/Objects/CCSceneObject.cpp \
                ../../Engine/Source/Objects/CCTile3D.cpp \
                ../../Engine/Source/Objects/CCTile3DButton.cpp \
\
                ../../Engine/Source/Rendering/CCMatrix.cpp \
                ../../Engine/Source/Rendering/CCModelBase.cpp \
                ../../Engine/Source/Rendering/CCModelText.cpp \
                ../../Engine/Source/Rendering/CCPrimitiveBase.cpp \
                ../../Engine/Source/Rendering/CCPrimitiveSquare.cpp \
                ../../Engine/Source/Rendering/CCRenderer.cpp \
                ../../Engine/Source/Rendering/CCRenderTools.cpp \
                ../../Engine/Source/Rendering/CCTextureBase.cpp \
                ../../Engine/Source/Rendering/CCTextureFontPage.cpp \
                ../../Engine/Source/Rendering/CCTextureFontPageFile.cpp \
                ../../Engine/Source/Rendering/CCTextureManager.cpp \
\
                ../../Engine/Source/Scenes/CCSceneAppUI.cpp \
                ../../Engine/Source/Scenes/CCSceneBase.cpp \
\
                ../../Engine/Source/Tools/CCBaseTools.cpp \
                ../../Engine/Source/Tools/CCBaseTypes.cpp \
                ../../Engine/Source/Tools/CCCallbacks.cpp \
                ../../Engine/Source/Tools/CCCollisionTools.cpp \
                ../../Engine/Source/Tools/CCFileManager.cpp \
                ../../Engine/Source/Tools/CCMathTools.cpp \
                ../../Engine/Source/Tools/CCOctree.cpp \
                ../../Engine/Source/Tools/CCURLManager.cpp \
                ../../Engine/Source/Tools/CCVectors.cpp \
\
                ../../App/Source/CCAppEngine.cpp \
                ../../App/Source/SceneSample.cpp \

INCLUDEPATH     +=  ../Source \
                    ../Source/Rendering \
                    ../Source/Tools \
\
                    ../../Engine/Source \
                    ../../Engine/Source/AI \
                    ../../Engine/Source/Objects \
                    ../../Engine/Source/Rendering \
                    ../../Engine/Source/Scenes \
                    ../../Engine/Source/Tools \
                    ../../Engine/Source/UI \
\
                    ../../App/Source \

HEADERS  	+= \
                ../Source/CCGLView.h \
                ../Source/CCMainWindow.h \
                ../Source/CCVideoView.h \
                ../Source/CCPlatform.h \
                ../Source/CCRenderThread.h \
                ../Source/Rendering/CCDeviceRenderer.h \
                ../Source/Rendering/CCTexturePNG.h \
                ../Source/Tools/CCDeviceFileManager.h \
                ../Source/Tools/CCDeviceURLManager.h \
\
                ../../Engine/Source/CCDefines.h \
                ../../Engine/Source/CCEngine.h \
                ../../Engine/Source/CCViewManager.h \
\
                ../../Engine/Source/AI/CCInterpolators.h \
\
                ../../Engine/Source/Objects/CCObjects.h \
                ../../Engine/Source/Objects/CCRenderable.h \
                ../../Engine/Source/Objects/CCSceneCollideable.h \
                ../../Engine/Source/Objects/CCSceneObject.h \
                ../../Engine/Source/Objects/CCTile3D.h \
                ../../Engine/Source/Objects/CCTile3DButton.h \
\
                ../../Engine/Source/Rendering/CCMatrix.h \
                ../../Engine/Source/Rendering/CCModelBase.h \
                ../../Engine/Source/Rendering/CCModelText.h \
                ../../Engine/Source/Rendering/CCPrimitiveBase.h \
                ../../Engine/Source/Rendering/CCPrimitives.h \
                ../../Engine/Source/Rendering/CCPrimitiveSquare.h \
                ../../Engine/Source/Rendering/CCRenderer.h \
                ../../Engine/Source/Rendering/CCRenderTools.h \
                ../../Engine/Source/Rendering/CCTextureBase.h \
                ../../Engine/Source/Rendering/CCTextureFontPage.h \
                ../../Engine/Source/Rendering/CCTextureFontPageFile.h \
                ../../Engine/Source/Rendering/CCTextureManager.h \
\
                ../../Engine/Source/Scenes/CCSceneAppUI.h \
                ../../Engine/Source/Scenes/CCSceneBase.h \
                ../../Engine/Source/Scenes/CCScenes.h \
\
                ../../Engine/Source/Tools/CCBaseTools.h \
                ../../Engine/Source/Tools/CCBaseTypes.h \
                ../../Engine/Source/Tools/CCCallbacks.h \
                ../../Engine/Source/Tools/CCCollisionTools.h \
                ../../Engine/Source/Tools/CCFileManager.h \
                ../../Engine/Source/Tools/CCMathTools.h \
                ../../Engine/Source/Tools/CCOctree.h \
                ../../Engine/Source/Tools/CCURLManager.h \
                ../../Engine/Source/Tools/CCVectors.h \
\
                ../../App/Source/CCAppEngine.h \
                ../../App/Source/Samples.h \
                ../../App/Source/SceneSample.h

RESOURCES 	+=	./2c.qrc
RC_FILE = myapp.rc

symbian:{

    LIBS += -lcone -leikcore -lavkon

    addResources.sources = ../../Engine/*
    addResources.path = ./Resources
    DEPLOYMENT += addResources
}

win32:{\

    copyfiles.commands += mkdir \"\" &
    copyfiles.commands += mkdir \"cache/api/\" &
    copyfiles.commands += mkdir \"cache/photos/\" &

# For Windows XP
    copyfiles.commands += xcopy \"../../Engine/Resources\" \"./Resources\" /s /y &
    copyfiles.commands += xcopy \"../../App/Resources\" \"Resources\" /s /y /i &

# For Parallels running Windows 7
    copyfiles.commands += cp -r ../../Engine/Resources/ ./ &
    copyfiles.commands += cp -r ../../App/Resources/ ./ &

}

macx:{

    copyfiles.commands += mkdir -p ./2c.app/Contents/MacOS/ &
    copyfiles.commands += mkdir -p ./2c.app/Contents/MacOS/cache/api/ &
    copyfiles.commands += mkdir -p ./2c.app/Contents/MacOS/cache/photos/ &

    copyfiles.commands += cp -r ../../Engine/Resources/ ./2c.app/Contents/MacOS/Resources/ &
    copyfiles.commands += cp -r ../../App/Resources/ ./2c.app/Contents/MacOS/Resources/ &

}

QMAKE_EXTRA_TARGETS += copyfiles
POST_TARGETDEPS += copyfiles
