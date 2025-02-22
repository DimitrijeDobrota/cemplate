if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include/cemplate-${PROJECT_VERSION}"
      CACHE STRING ""
  )
  set_property(CACHE CMAKE_INSTALL_INCLUDEDIR PROPERTY TYPE PATH)
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package cemplate)

install(
    DIRECTORY
    include/
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT cemplate_Development
)

install(
    TARGETS cemplate_cemplate
    EXPORT cemplateTargets
    RUNTIME #
    COMPONENT cemplate_Runtime
    LIBRARY #
    COMPONENT cemplate_Runtime
    NAMELINK_COMPONENT cemplate_Development
    ARCHIVE #
    COMPONENT cemplate_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    cemplate_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE STRING "CMake package config location relative to the install prefix"
)
set_property(CACHE cemplate_INSTALL_CMAKEDIR PROPERTY TYPE PATH)
mark_as_advanced(cemplate_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${cemplate_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT cemplate_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${cemplate_INSTALL_CMAKEDIR}"
    COMPONENT cemplate_Development
)

install(
    EXPORT cemplateTargets
    NAMESPACE cemplate::
    DESTINATION "${cemplate_INSTALL_CMAKEDIR}"
    COMPONENT cemplate_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
