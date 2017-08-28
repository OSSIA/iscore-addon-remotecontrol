cmake_minimum_required(VERSION 3.1)
project(iscore_addon_remotecontrol LANGUAGES CXX)

###  General initialization of always necessary stuff (AUTOMOC, etc) ###
iscore_common_setup()

### Files & main target ###
set(SRCS
"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/Settings/Model.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/Settings/Model.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/Settings/Presenter.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/Settings/Presenter.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/Settings/View.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/Settings/View.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/Settings/Factory.hpp"

"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/Scenario/Process.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/Scenario/Scenario.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/Scenario/Loop.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/Scenario/Constraint.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/Scenario/Event.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/Scenario/Sync.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/Scenario/State.hpp"

"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/Scenario/Process.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/Scenario/Scenario.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/Scenario/Loop.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/Scenario/Constraint.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/Scenario/Event.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/Scenario/Sync.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/Scenario/State.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/ApplicationPlugin.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/ApplicationPlugin.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/DocumentPlugin.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/RemoteControl/DocumentPlugin.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/iscore_addon_remotecontrol.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/iscore_addon_remotecontrol.hpp"
)

add_library(${PROJECT_NAME} ${SRCS} ${HDRS} ${QRCS})

target_link_libraries(${PROJECT_NAME} PUBLIC iscore_plugin_scenario iscore_plugin_loop Qt5::WebSockets)

setup_iscore_plugin(${PROJECT_NAME})

### Setup of the target ###
#TODO
#function(setup_iscore_addon AddonTarget)
#    set(options)
#    set(oneValueArgs NAME METADATA)
#    set(multiValueArgs)
#    cmake_parse_arguments(SETUP_ADDON "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

#    setup_iscore_common_lib_features("${AddonTarget}")

#    set(ADDON_FOLDER "${CMAKE_BINARY_DIR}/addons/${SETUP_ADDON_NAME}/")
#    set(ADDON_PLATFORM "${ISCORE_ADDON_PLATFORM}")
#    set(ADDON_FILENAME "${SETUP_ADDON_NAME}-${ISCORE_ADDON_SUFFIX}")

#    set_target_properties(${AddonTarget} PROPERTIES
#        LIBRARY_OUTPUT_DIRECTORY "${ADDON_FOLDER}/"
#        PREFIX ""
#        SUFFIX ""
#        OUTPUT_NAME "${ADDON_FILENAME}")
#    configure_file("${SETUP_ADDON_METADATA}" "${ADDON_FOLDER}/localaddon.json")

#    iscore_cotire_post("${AddonTarget}")
#endfunction()

#setup_iscore_addon(
#    ${PROJECT_NAME}
#    NAME RemoteControl
#    METADATA "${CMAKE_CURRENT_SOURCE_DIR}/Deployment/localaddon.json.in"
#     )
