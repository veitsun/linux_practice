find_program(KERNEL_GNU_MAKE NAMES make gmake REQUIRED)

execute_process(
    COMMAND uname -r
    OUTPUT_VARIABLE KERNEL_RELEASE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(KERNEL_MODULE_NAME kernel_lab)
set(KERNEL_BUILD_DIR /lib/modules/${KERNEL_RELEASE}/build)
set(KERNEL_MODULE_OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${KERNEL_MODULE_NAME}.ko)

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
        COMMAND ${CMAKE_COMMAND} -E echo "Load command   : sudo insmod ${KERNEL_MODULE_OUTPUT} greeting=hi repeat=3"
        COMMAND ${CMAKE_COMMAND} -E echo "Read procfs    : cat /proc/kernel_lab"
        COMMAND ${CMAKE_COMMAND} -E echo "Write procfs   : echo demo | sudo tee /proc/kernel_lab"
        COMMAND ${CMAKE_COMMAND} -E echo "Unload command : sudo rmmod ${KERNEL_MODULE_NAME}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Printing usage for ${KERNEL_MODULE_NAME}"
        VERBATIM
    )
else()
    message(WARNING "Kernel headers were not found at ${KERNEL_BUILD_DIR}; kernel_lab targets are disabled.")
endif()
