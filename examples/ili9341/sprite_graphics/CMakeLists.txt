set(TARGET_NAME mode1_demo)

add_executable(${TARGET_NAME}
        ${TARGET_NAME}.c
        )

target_link_libraries(${TARGET_NAME} PRIVATE
        mode1
        ili9341
        pico_stdlib
        hardware_spi
        )

pico_enable_stdio_usb(${TARGET_NAME} 1)
pico_enable_stdio_uart(${TARGET_NAME} 0)

pico_add_extra_outputs(${TARGET_NAME})