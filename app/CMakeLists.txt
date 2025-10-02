add_executable(${EXECUTABLE_NAME_NOIPC} "blur_noipc.cpp")
target_link_libraries(${EXECUTABLE_NAME_NOIPC} PRIVATE ${IMAGE_IO_LIBRARY} ${GAUSSIAN_BLUR_LIBRARY})

add_executable(${EXECUTABLE_NAME_IPC} "blur_ipc.cpp")
target_link_libraries(
        ${EXECUTABLE_NAME_IPC} PUBLIC
        ${IMAGE_IO_LIBRARY}
        ${GAUSSIAN_BLUR_LIBRARY}
        ${IPC_MANAGER_LIBRARY}
        ${TILE_MANAGER_LIBRARY}
)
