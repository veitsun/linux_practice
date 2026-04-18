find_program(KERNEL_GNU_MAKE NAMES make gmake REQUIRED)
find_program(BEAR_EXECUTABLE NAMES bear)

execute_process(
    COMMAND uname -r
    OUTPUT_VARIABLE KERNEL_RELEASE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(KERNEL_MODULE_NAME kernel_lab)
set(KERNEL_BUILD_DIR /lib/modules/${KERNEL_RELEASE}/build)
set(KERNEL_MODULE_OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${KERNEL_MODULE_NAME}.ko)
set(KERNEL_MODULE_COMPILE_COMMANDS ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json)

if(BEAR_EXECUTABLE)
    execute_process(
        COMMAND ${BEAR_EXECUTABLE} --help
        OUTPUT_VARIABLE BEAR_HELP_TEXT
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if(BEAR_HELP_TEXT MATCHES "--output")
        set(BEAR_CDB_FLAG --output)
    elseif(BEAR_HELP_TEXT MATCHES "--cdb")
        set(BEAR_CDB_FLAG --cdb)
    else()
        set(BEAR_CDB_FLAG -o)
    endif()
endif()

if(EXISTS ${KERNEL_BUILD_DIR})
    add_custom_target(
        kernel_lab_module
        COMMAND ${KERNEL_GNU_MAKE} -C ${KERNEL_BUILD_DIR} M=${CMAKE_CURRENT_SOURCE_DIR} modules
        BYPRODUCTS ${KERNEL_MODULE_OUTPUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Building ${KERNEL_MODULE_NAME}.ko against Linux ${KERNEL_RELEASE}"
        VERBATIM
    )

    add_custom_target(
        kernel_lab_clean
        COMMAND ${KERNEL_GNU_MAKE} -C ${KERNEL_BUILD_DIR} M=${CMAKE_CURRENT_SOURCE_DIR} clean
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Cleaning kernel module artifacts for ${KERNEL_MODULE_NAME}"
        VERBATIM
    )

    add_custom_target(
        kernel_lab_help
        COMMAND ${CMAKE_COMMAND} -E echo "Kernel release : ${KERNEL_RELEASE}"
        COMMAND ${CMAKE_COMMAND} -E echo "Module output  : ${KERNEL_MODULE_OUTPUT}"
        COMMAND ${CMAKE_COMMAND} -E echo "Build target   : cmake --build <build-dir> --target kernel_lab_module"
        COMMAND ${CMAKE_COMMAND} -E echo "Clangd DB      : cmake --build <build-dir> --target kernel_lab_compdb"
        COMMAND ${CMAKE_COMMAND} -E echo "Load command   : sudo insmod ${KERNEL_MODULE_OUTPUT} greeting=hi repeat=3"
        COMMAND ${CMAKE_COMMAND} -E echo "Read procfs    : cat /proc/kernel_lab"
        COMMAND ${CMAKE_COMMAND} -E echo "Write procfs   : echo demo | sudo tee /proc/kernel_lab"
        COMMAND ${CMAKE_COMMAND} -E echo "Unload command : sudo rmmod ${KERNEL_MODULE_NAME}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Printing usage for ${KERNEL_MODULE_NAME}"
        VERBATIM
    )

    if(BEAR_EXECUTABLE)
        add_custom_target(
            kernel_lab_compdb
            COMMAND ${KERNEL_GNU_MAKE} -C ${KERNEL_BUILD_DIR} M=${CMAKE_CURRENT_SOURCE_DIR} clean
            COMMAND ${BEAR_EXECUTABLE} ${BEAR_CDB_FLAG} ${KERNEL_MODULE_COMPILE_COMMANDS}
                    ${KERNEL_GNU_MAKE} -C ${KERNEL_BUILD_DIR} M=${CMAKE_CURRENT_SOURCE_DIR} modules
            BYPRODUCTS ${KERNEL_MODULE_COMPILE_COMMANDS}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating compile_commands.json for ${KERNEL_MODULE_NAME} with Bear"
            VERBATIM
        )
    else()
        add_custom_target(
            kernel_lab_compdb
            COMMAND ${CMAKE_COMMAND} -E echo "bear was not found. Install it first, e.g. 'sudo apt install bear'."
            COMMAND ${CMAKE_COMMAND} -E false
            COMMENT "Bear is required to generate kernel module compile_commands.json"
            VERBATIM
        )
    endif()
else()
    message(WARNING "Kernel headers were not found at ${KERNEL_BUILD_DIR}; kernel_lab targets are disabled.")
endif()
