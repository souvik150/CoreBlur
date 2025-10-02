## CoreBlur ⚡

CoreBlur is a high-performance image processing project in C++ that applies Gaussian Blur while experimenting with:

- IPC (Inter-Process Communication) for parallel image handling
- Core Pinning to performance cores for predictable latency
- Cache-friendly memory layouts for speed optimization

---

The project serves as a benchmark playground to compare custom Gaussian blur implementations against optimized library functions (like OpenCV), helping measure how low-level optimizations affect runtime.

---

Instructions to build :- 

`
./build.sh
`

Instructions to run :- 

`
./build/app/{executable_name} old.jpeg new.jpeg
`

Results :- 

```
❯ ./build/app/GausssianBlurIPC old.jpeg new.jpeg 
Parallel blur completed in 866.377 ms

❯ ./build/app/GausssianBlur old.jpeg new.jpeg 
Gaussian blur completed in 2183.08 ms
```