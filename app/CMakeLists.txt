add_executable(${EXECUTABLE_NAME} "main.cpp")
target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${IMAGE_IO_LIBRARY} ${GAUSSIAN_BLUR_LIBRARY})
