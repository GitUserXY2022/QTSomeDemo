win32:CONFIG(release, debug|release){
    LIBS += -L"$$PWD/../3rdparty/ctk2/lib/ctk-0.1_release"
}
else:win32:CONFIG(debug, debug|release){
    DEFINES += __DEBUG__
    LIBS += -L"$$PWD/../3rdparty/ctk2/lib/ctk-0.1_debug"
}

QT += core gui widgets sql xml network multimedia websockets
DEFINES += _WINDOWS WIN64 x64 WIN64 HAVE_QT5 ASDP CV_IGNORE_DEBUG_BUILD_GUARD

QMAKE_CXXFLAGS_EXCEPTIONS_ON = /EHa
QMAKE_CXXFLAGS_STL_ON = /EHa
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

INCLUDEPATH += $$PWD/../3rdparty/ctk2/include/ctk-0.1 \
        $$PWD/../3rdparty/evgx/GX_DTarget/Include \
        $$PWD/../3rdparty/evgx/GX_DTarget/dependences/Inc \
        $$PWD/../3rdparty/evgx/GX_CGraphics/Include \
        $$PWD/../3rdparty/opencv/include \
        "$$(EV_GLOBESDK_DEBUGDLL64)/../include" \
        $$PWD/include

LIBS +=  \
    -L"$$PWD/../3rdparty/opencv/lib/x64" \
    -lCTKCore \
    -lCTKPluginFramework
