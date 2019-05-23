# Source: https://github.com/Andersbakken/rtags/blob/master/cmake/BashCompletion.cmake

set(FORCE_BASH_COMPLETION_INSTALLATION FALSE CACHE BOOL "Force bash completion installation")

if(FORCE_BASH_COMPLETION_INSTALLATION AND "${BASH_COMPLETION_COMPLETIONSDIR}" STREQUAL "")
    set(BASH_COMPLETION_COMPLETIONSDIR "/share/bash-completion/completions")
endif()

find_package(PkgConfig QUIET)
set_package_properties(PkgConfig
    PROPERTIES
    URL "https://www.freedesktop.org/wiki/Software/pkg-config/"
    DESCRIPTION "helper tool"
    TYPE OPTIONAL
    PURPOSE "We use it to get the bash completion installation path, and replace the prefix with the value of CMAKE_INSTALL_PREFIX.")
if(PKG_CONFIG_FOUND OR FORCE_BASH_COMPLETION_INSTALLATION)
    if(PKG_CONFIG_FOUND AND NOT FORCE_BASH_COMPLETION_INSTALLATION)
        execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=completionsdir bash-completion
            RESULT_VARIABLE BASH_COMPLETION_UNAVAILABLE
            OUTPUT_VARIABLE BASH_COMPLETION_COMPLETIONSDIR
            ERROR_QUIET
            OUTPUT_STRIP_TRAILING_WHITESPACE)
	if(NOT BASH_COMPLETION_UNAVAILABLE)
	    execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=prefix bash-completion
		OUTPUT_VARIABLE BASH_COMPLETION_COMPLETIONSDIR_PREFIX
		ERROR_QUIET
		OUTPUT_STRIP_TRAILING_WHITESPACE)
	    string(REPLACE "${BASH_COMPLETION_COMPLETIONSDIR_PREFIX}" "" BASH_COMPLETION_COMPLETIONSDIR ${BASH_COMPLETION_COMPLETIONSDIR})
	endif()
    endif()
    if(NOT BASH_COMPLETION_UNAVAILABLE OR FORCE_BASH_COMPLETION_INSTALLATION)
	set(BASH_COMPLETION_COMPLETIONSDIR "${CMAKE_INSTALL_PREFIX}${BASH_COMPLETION_COMPLETIONSDIR}" CACHE PATH "Bash completion installation directory" FORCE)
        mark_as_advanced(BASH_COMPLETION_COMPLETIONSDIR)
        set(BASH_COMPLETION_FOUND TRUE)
    endif()
endif()
