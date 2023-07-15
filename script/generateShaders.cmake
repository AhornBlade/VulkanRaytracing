function(generate_shaders SHADER_DIR TARGET_NAME)
	set(SPV_DIR ${CMAKE_CURRENT_BINARY_DIR}/spv)
	set(HEADER_DIR ${CMAKE_CURRENT_BINARY_DIR}/header)
	set(GLSLANG_BIN $ENV{VK_SDK_PATH}/Bin/glslangValidator.exe)
	file(GLOB SHADERS ${SHADER_DIR}/*.vert ${SHADER_DIR}/*.frag ${SHADER_DIR}/*.comp ${SHADER_DIR}/*.geom ${SHADER_DIR}/*.tesc ${SHADER_DIR}/*.tese ${SHADER_DIR}/*.mesh ${SHADER_DIR}/*.task ${SHADER_DIR}/*.rgen ${SHADER_DIR}/*.rchit ${SHADER_DIR}/*.rmiss)
	
	foreach(SHADER IN LISTS SHADERS)
		get_filename_component(SHADER_NAME ${SHADER} NAME)
		string(REPLACE "." "_" HEADER_NAME ${SHADER_NAME})
		string(TOUPPER ${HEADER_NAME} GLOBAL_SHADER_VAR)
		set(SPV_FILE "${SPV_DIR}/${SHADER_NAME}.spv")
		set(SPV_TARGET "${SHADER_NAME}_SPV")
		set(HEADER_FILE "${HEADER_DIR}/${HEADER_NAME}.hpp")
		set(HEADER_TARGET "${SHADER_NAME}_HEADER")

		add_custom_command(OUTPUT ${SPV_FILE}
			COMMAND ${GLSLANG_BIN} -I${SHADER_DIR} -V100 -o ${SPV_FILE} ${SHADER}
			DEPENDS ${SHADER}
			COMMENT "Compiling ${FILENAME}")
		add_custom_target(${SPV_TARGET} DEPENDS ${SPV_FILE})

		add_custom_command(OUTPUT ${HEADER_FILE}
			COMMAND ${VULKAN_RAYTRACING_SCRIPT_DIR}/generate.py ${SPV_FILE} ${HEADER_FILE}
			DEPENDS ${SPV_FILE}
			COMMENT "Generating ${HEADER_NAME}")
		add_custom_target(${HEADER_TARGET} DEPENDS ${HEADER_FILE})
		add_dependencies(${HEADER_TARGET} ${SPV_TARGET})

		add_dependencies(${TARGET_NAME} ${HEADER_TARGET})
	endforeach()

	target_include_directories(${TARGET_NAME}
		PUBLIC ${HEADER_DIR})

endfunction()