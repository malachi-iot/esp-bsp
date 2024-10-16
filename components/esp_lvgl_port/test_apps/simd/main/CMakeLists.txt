set(ADD_SRCS "")
set(ADD_DIRS "")

# Hard copy of LV files
file(GLOB_RECURSE BLEND_SRCS lv_blend/src/*.c)
list(APPEND ADD_SRCS ${BLEND_SRCS})

# Include SIMD assembly source code for rendering
if(CONFIG_IDF_TARGET_ESP32 OR CONFIG_IDF_TARGET_ESP32S3)
    message(VERBOSE "Compiling SIMD")
    set(PORT_PATH "../../../src/lvgl9")

    if(CONFIG_IDF_TARGET_ESP32S3)
        file(GLOB_RECURSE ASM_SRCS ${PORT_PATH}/simd/*_esp32s3.S)    # Select only esp32s3 related files
    else()
        file(GLOB_RECURSE ASM_SRCS ${PORT_PATH}/simd/*_esp32.S)      # Select only esp32 related files
    endif()
    list(APPEND ADD_SRCS ${ASM_SRCS})

    file(GLOB_RECURSE ASM_SRCS_COMMON ${PORT_PATH}/simd/*_common.S   # Include common assembly src files
                                      ${PORT_PATH}/simd/*_macro.S)   # Include assembly macros
    
    list(APPEND ADD_SRCS ${ASM_SRCS_COMMON})
else()
    message(WARNING "This test app is intended only for esp32 and esp32s3")
endif()

list(APPEND ADD_SRCS "test_app_main.c"
                     "test_lv_fill_functionality.c"
                     "test_lv_fill_benchmark.c"
                     "test_functionality_eval.c")

list(APPEND ADD_DIRS "lv_blend/include"     # Hard copy of LVGL files headers
                      "../../../include")   # esp_lvgl port headers

idf_component_register(SRCS ${ADD_SRCS}
                      INCLUDE_DIRS ${ADD_DIRS}
                      REQUIRES unity
                      WHOLE_ARCHIVE)
