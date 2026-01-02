# YC-Skyguard-Pro 📡

A multi-threaded Radar Simulation written in C++. 

I built this project to practice **concurrency** (std::thread, std::mutex) and modern C++ memory management concepts while simulating a real-time system.

## Features
- **Multi-threaded Scanning:** The radar runs on a separate thread, allowing the main UI thread to remain responsive.
- **Thread Safety:** Uses `std::mutex` to handle console outputs safely.
- **Simulation:** Generates random targets and calculates distances using basic geometry.

## How to Run
```bash
mkdir build && cd build
cmake ..
cmake --build .
./Skyguard