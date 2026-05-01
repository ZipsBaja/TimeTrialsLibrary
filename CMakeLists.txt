set(LIBRARY_NAME pico-timetrials-lib)

if (NOT TARGET ${LIBRARY_NAME})

    add_library(${LIBRARY_NAME} STATIC
        # Add your source files here
        src/devices/TimeTrials.cpp

    )
    add_subdirectory(lib/ZipsPicoLib)
    add_subdirectory(lib/pico-ssd1306-ui)

    target_link_libraries(${LIBRARY_NAME}

        ZipsPicoLib
        pico-ssd1306-ui
    )
    target_include_directories(${LIBRARY_NAME} PUBLIC
        include
    )

endif()