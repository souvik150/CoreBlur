# CoreBlur ⚡

**CoreBlur** is a high-performance C++ image processing project that implements **Gaussian Blur** while exploring low-level optimizations and parallelism strategies:

* **IPC (Inter-Process Communication)** for splitting large images into tiles and processing them in parallel.
* **Core Pinning** to performance cores for deterministic execution and low latency.
* **Cache-friendly memory layouts** and shared memory to maximize throughput.
* Comparison with optimized libraries (e.g., **OpenCV**) for benchmarking custom implementations.

---

## Directory Structure

```
souvik150-coreblur/
├── README.md
├── build.sh
├── CMakeLists.txt
├── app/
│   ├── blur_ipc.cpp
│   ├── blur_ipc_pinning.cpp
│   ├── blur_noipc.cpp
│   ├── blur_opencv.cpp
│   └── CMakeLists.txt
├── cmake/
│   ├── AddGitSubmodule.cmake
│   └── CPM.cmake
├── external/
│   └── CMakeLists.txt
├── src/
│   ├── CMakeLists.txt
│   ├── core_affinity/
│   │   ├── CMakeLists.txt
│   │   ├── ipc_manager.cpp
│   │   └── ipc_manager.h
│   ├── gaussian_blur/
│   │   ├── CMakeLists.txt
│   │   ├── gaussian_blur.cpp
│   │   └── gaussian_blur.h
│   ├── image_io/
│   │   ├── CMakeLists.txt
│   │   ├── image_io.cpp
│   │   └── image_io.h
│   ├── ipc/
│   │   ├── CMakeLists.txt
│   │   ├── shm_manager.cpp
│   │   └── shm_manager.h
│   └── tile_manager/
│       ├── CMakeLists.txt
│       ├── tile_manager.cpp
│       └── tile_manager.h
```

---

## Build Instructions

```bash
./build.sh
```

## Run Instructions

```bash
./build/app/<executable_name> <input_image> <output_image>
```

### Example Runs

```
❯ ./build/app/GaussianBlurIPC old.jpeg new.jpeg 
Parallel blur completed in 866.377 ms

❯ ./build/app/GaussianBlur old.jpeg new.jpeg 
Gaussian blur completed in 2183.08 ms
```

---

## Notes

* `blur_ipc.cpp` and `blur_ipc_pinning.cpp` demonstrate parallel processing using shared memory and core pinning.
* `blur_noipc.cpp` is a single-threaded baseline.
* `blur_opencv.cpp` uses OpenCV for comparison.
* `src/ipc/shm_manager.*` handles shared memory creation, reading, and cleanup.
* `src/tile_manager/*` splits images into tiles for parallel processing.
* `src/image_io/*` contains utilities for loading and saving images.
* `src/core_affinity/*` contains code for CPU core pinning.
* `src/gaussian_blur/*` contains the Gaussian blur implementation.

---

## License

MIT License.
