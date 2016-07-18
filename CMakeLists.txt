if(WIN32)
    cmake_minimum_required(VERSION 3.1.0)
    set(CMAKE_CXX_STANDARD 11)
else()
    cmake_minimum_required(VERSION 2.8.12)
    add_compile_options(-std=c++11)
endif()

# --- BUILD CONFIGURATION ---

project(HorizonLauncher C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(SOURCE
    # MAIN
    src/main.cpp
    src/widgets/mainpanel.cpp

    # NAVBAR
    src/widgets/navbar/navbar.cpp
    src/widgets/navbar/tabwidget.cpp
    src/widgets/navbar/tablabel.cpp

    # STACKED WIDGETS
    src/widgets/stack/library.cpp
    src/widgets/stack/homepage.cpp
    src/widgets/stack/news.cpp
    src/widgets/stack/community.cpp
    src/widgets/stack/store.cpp
    src/widgets/stack/settings.cpp

    # OTHER WIDGETS
    src/widgets/newsitemwidget.cpp
    src/widgets/gridgamewidget.cpp
    src/widgets/buddylistexpandable.cpp
    src/widgets/newsfeedchooserwindow.cpp
    src/widgets/detailgamewidget.cpp
    src/widgets/buddylist.cpp
    src/widgets/gamepropertyeditor.cpp

    # LIBRARY VIEWS
    src/widgets/views/librarygridview.cpp
    src/widgets/views/librarydetailview.cpp
    src/widgets/views/librarycarouselview.cpp

    # WIZARDS
    src/widgets/wizards/drmsetupwizard.cpp
    src/widgets/wizards/addgamewizard.cpp
    src/widgets/wizards/setupwizard.cpp

    # STEAM
    src/steam/steamhelper.cpp
    src/steam/steammetadatahelper.cpp

    # MISC
    src/database.cpp
    src/gamelauncher.cpp
)

set(RESOURCES
    res/icon.qrc
    res/font.qrc
    res/styles.qrc
    res/image.qrc
)

set(GUI_TYPE "")

set(TRANSLATIONS
    Translations/horizon_da.ts
    Translations/horizon_nb.ts
    Translations/horizon_pt_BR.ts
    Translations/horizon_fr.ts
    Translations/horizon_de.ts
    Translations/horizon_es.ts
    Translations/horizon_pl.ts
)

if(WIN32)
    #Additional configuration for Windows systems.
    # TODO: Get working "Release" config for multi-configuration MSVC builds.

    # set(GUI_TYPE WIN32)

    set(SOURCE ${SOURCE}
        src/widgets/window/borderlesswindow.cpp
        src/widgets/window/winwindow.cpp
        src/widgets/window/winpanel.cpp
        BorderlessWindow/QWinHost.cpp
        BorderlessWindow/QWinWidget.cpp
    )
elseif(UNIX)
    # Additional configuration for Unix systems.
    # TODO: Get working icon / bundle for OSX systems without bricking the build.

    # if(APPLE)
    #     set(GUI_TYPE MACOSX_BUNDLE)
    # endif(APPLE)

    set(SOURCE ${SOURCE}
        src/widgets/window/unixwindow.cpp
        src/widgets/window/unixpanel.cpp
    )
else()
    # Suspend the configuration process if the target OS isn't recognized.
    MESSAGE(FATAL_ERROR "Target OS not recognized or supported, aborting CMake process.")
endif()

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5WebKit REQUIRED)
find_package(Qt5WebKitWidgets REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Boost REQUIRED)
find_package(Threads REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

qt5_add_resources(UI_RESOURCES ${RESOURCES})
qt5_add_translation(TRANSLATIONS ${TRANSLATIONS})
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Network_INCLUDE_DIRS}
    ${Qt5WebKit_INCLUDE_DIRS}
    ${Qt5WebKitWidgets_INCLUDE_DIRS}
    ${Qt5Sql_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    BorderlessWindow
    src
)
add_executable(HorizonLauncher
    ${GUI_TYPE}
    ${SOURCE}
    ${OS_SOURCE}
    ${UI_RESOURCES}
    ${TRANSLATIONS}
)
target_link_libraries(HorizonLauncher
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    Qt5::Network
    Qt5::WebKit
    Qt5::WebKitWidgets
    Qt5::Sql
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

if(WIN32)
    target_link_libraries(HorizonLauncher
        user32.lib
        gdi32.lib
        dwmapi.lib
    )

    if(MSVC)
        set(WIN_LINKFLAGS \"${CMAKE_CURRENT_SOURCE_DIR}/res/launcher.res\")
        set_target_properties(HorizonLauncher PROPERTIES LINK_FLAGS ${WIN_LINKFLAGS})
        set_target_properties(HorizonLauncher PROPERTIES COMPILE_FLAGS "/EHsc")
    endif(MSVC)
endif()

# --- RUNTIME DEPENDENCIES ---
# Only necessary on Windows.

if(WIN32)
    cmake_policy(SET CMP0026 OLD)

    get_target_property(BuildLocation HorizonLauncher LOCATION)
    get_filename_component(TargetDirectory ${BuildLocation} PATH)

    if (EXISTS ${Qt5Core_DIR}/../../../bin/icuin53.dll)
        set(ICU_VER 53)
    else()
        set(ICU_VER 54)
    endif()

    foreach(DEPENDENCY
        Qt5Widgetsd.dll
        Qt5Guid.dll
        Qt5Cored.dll
        Qt5Networkd.dll
        Qt5WebKitd.dll
        Qt5WebKitWidgetsd.dll
        Qt5Sqld.dll
        Qt5Sensorsd.dll
        Qt5Positioningd.dll
        Qt5Quickd.dll
        Qt5Qmld.dll
        Qt5Multimediad.dll
        Qt5MultimediaWidgetsd.dll
#       Qt5WebChanneld.dll
        Qt5Svgd.dll
        Qt5OpenGLd.dll
        Qt5PrintSupportd.dll
        icuin${ICU_VER}.dll
        icuuc${ICU_VER}.dll
        icudt${ICU_VER}.dll
    )
        add_custom_command(TARGET HorizonLauncher POST_BUILD
            # Copy the given dependency into the executable directory.
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Qt5Core_DIR}/../../../bin/${DEPENDENCY} ${TargetDirectory}
            COMMENT "Copying ${DEPENDENCY} into '${TargetDirectory}/'" VERBATIM
        )
    endforeach()
endif()
