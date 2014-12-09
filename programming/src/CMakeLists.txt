add_subdirectory(libraries)

OPTION(TEST "Test for libraries and integration")
IF(TEST)
add_subdirectory(test)
ENDIF()

OPTION(APPS "Applications of the laser tower")
IF(APPS)
add_subdirectory(apps)
ENDIF()



