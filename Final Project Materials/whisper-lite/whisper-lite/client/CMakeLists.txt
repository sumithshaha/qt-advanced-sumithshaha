cmake_minimum_required(VERSION 3.21)
project(whisper VERSION 1.0.0 LANGUAGES CXX)

# -------- Build settings ------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# -------- Qt 6.10+ -------------------------------------------------------
find_package(Qt6 6.5 REQUIRED COMPONENTS
    Core
    Gui
    Quick
    QuickControls2
    WebSockets
    Network
    Sql
)

qt_standard_project_setup(REQUIRES 6.5)

# Note: WHISPER_QML_BUILD is set per-target below (only on appwhisper),
# NOT globally. Otherwise the test target also gets it, and MOC tries to
# include <QtQmlIntegration> while compiling tst_store — which doesn't
# link Qt Quick. Same headers, two compile contexts.

# -------- Source list ---------------------------------------------------
set(WHISPER_SOURCES
    src/main.cpp
    src/wsclient.cpp        src/wsclient.h
    src/tlsconfig.cpp       src/tlsconfig.h
    src/messagestore.cpp    src/messagestore.h
    src/messagemodel.cpp    src/messagemodel.h
    src/usermodel.cpp       src/usermodel.h
    src/chatcontroller.cpp  src/chatcontroller.h
    src/settings.cpp        src/settings.h
    src/notificationhelper.cpp src/notificationhelper.h
)

# -------- Executable ----------------------------------------------------
qt_add_executable(appwhisper ${WHISPER_SOURCES})

# Only the main app sees WHISPER_QML_BUILD — tests must NOT see it.
target_compile_definitions(appwhisper PRIVATE WHISPER_QML_BUILD)

# -------- Qt resources --------------------------------------------------
# Pinned CA cert + SPKI pin baked into the binary so they're available
# at :/whisper_ca.pem and :/spki_pin.txt at runtime.
qt_add_resources(appwhisper "whisper_resources"
    PREFIX "/"
    BASE "${CMAKE_CURRENT_SOURCE_DIR}/android/res/raw"
    FILES
        android/res/raw/whisper_ca.pem
        android/res/raw/spki_pin.txt
)

# -------- QML module ----------------------------------------------------
qt_add_qml_module(appwhisper
    URI Whisper
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/LoginPage.qml
        qml/ChatPage.qml
        qml/MessageDelegate.qml
        qml/StatusIndicator.qml
)

target_include_directories(appwhisper PRIVATE src)

target_link_libraries(appwhisper PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::WebSockets
    Qt6::Network
    Qt6::Sql
)

# -------- Android packaging --------------------------------------------
if(ANDROID)
    set_target_properties(appwhisper PROPERTIES
        QT_ANDROID_PACKAGE_SOURCE_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/android"
        QT_ANDROID_TARGET_SDK_VERSION  35
        QT_ANDROID_MIN_SDK_VERSION     26
        QT_ANDROID_VERSION_NAME        "${PROJECT_VERSION}"
        QT_ANDROID_VERSION_CODE        1
    )

    # Workaround for Android 16 KB page-size warning with NDK r27 (QTBUG-144166).
    target_link_options(appwhisper PRIVATE "-Wl,-z,max-page-size=16384")
endif()

# -------- Tests ---------------------------------------------------------
option(WHISPER_BUILD_TESTS "Build unit tests" ON)
if(WHISPER_BUILD_TESTS AND NOT ANDROID)
    enable_testing()
    add_subdirectory(tests)
endif()