macro(add_dml_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} dmlCore)
    add_test(NAME ${name} COMMAND ${name})
endmacro(add_dml_test name)

add_dml_test(test_core_video)
add_dml_test(test_core_video_list)
add_dml_test(test_core_audio)
add_dml_test(test_core_audio_list)

