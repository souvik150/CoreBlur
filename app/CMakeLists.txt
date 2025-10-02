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

add_executable(${EXECUTABLE_NAME_IPC_CORE_PINNING} "blur_ipc_pinning.cpp")
target_link_libraries(
        ${EXECUTABLE_NAME_IPC_CORE_PINNING} PUBLIC
        ${IMAGE_IO_LIBRARY}
        ${GAUSSIAN_BLUR_LIBRARY}
        ${IPC_MANAGER_LIBRARY}
        ${TILE_MANAGER_LIBRARY}
        ${CORE_AFFINITY_MANAGER_LIBRARY}
)


add_executable(${EXECUTABLE_NAME_OPENCV} "blur_opencv.cpp")
target_link_libraries(${EXECUTABLE_NAME_OPENCV} PRIVATE ${OpenCV_LIBS})
target_include_directories(${EXECUTABLE_NAME_OPENCV} PRIVATE ${OpenCV_INCLUDE_DIRS})