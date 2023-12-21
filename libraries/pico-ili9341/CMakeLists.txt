add_library(ili9341
	font.c
	ili9341.c
)

target_include_directories(ili9341 PUBLIC 
	${ILI9341_DIR}/include
	)

target_link_libraries(ili9341
	pico_stdlib
	hardware_spi)



# MODE 1 (SPRITE GRAPHICS)
add_library(mode1
	mode1.c
)

target_include_directories(mode1 PUBLIC
	${ILI9341_DIR}/include
	)

target_link_libraries(mode1
	ili9341
	pico_stdlib
	hardware_spi)

# MODE 2 (FRAMEBUFFER)
add_library(mode2
	mode2.c
)

target_include_directories(mode2 PUBLIC
	${ILI9341_DIR}/include
	)

target_link_libraries(mode2
	ili9341
	pico_stdlib
	hardware_spi)


# MODE 0 (TEXT)
add_library(mode0
	mode0.c
)

target_include_directories(mode0 PUBLIC
	${ILI9341_DIR}/include
	)

target_link_libraries(mode0
	ili9341
	pico_stdlib
	hardware_spi)



