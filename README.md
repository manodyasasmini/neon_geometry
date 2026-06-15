# Neon Geometry: Protocol

A minimalistic, cinematic tech-aesthetic 2D survival arena shooter built from scratch in C++ using OpenGL and the GLUT utility toolkit. This repository showcases structural computer graphics fundamentals distributed across a 6-member engineering team.

---

## 👥 6-Member Responsibility Breakdown

To avoid workflow conflicts, the engine architecture isolates individual math pipelines into standalone modular codebases:

* **Member 1 (Player Core):** Manages compound matrix operations (`glTranslate`, `glRotate`, `glScale`) driving the primary entity and local dynamic thruster oscillations. Optimized with state-based continuous key tracking for fluid movement.
* **Member 2 (The Swarm):** Implements automated targeted vector pathways enabling AI groups to seek and translate smoothly toward the player's real-time coordinate state. Speed factors balanced for smoother gameplay.
* **Member 3 (Laser Projectiles):** Bypasses basic hardware line-loops to execute an academic **Digital Differential Analyzer (DDA) / Bresenham's rasterization algorithm** plotting high-speed bullet points manually.
* **Member 4 (The Pulse Grid):** Owns background projection. Combines a smooth dynamic parallax scrolling effect (`glTranslate` relative to player movement) with custom Row-Major 4x4 Shearing Matrices to map dynamic warp tilt exclusively during high-speed dashes.
* **Member 5 (The Anomaly Boss):** Orchestrates procedural parametric equations (`sin`/`cos`) deforming geometry vertices dynamically over time to create an unstable morphing boss variant. The boss becomes active and visible starting at Level 3, and features an animated pulsing inner core.
* **Member 6 (Particle Explosions):** Deploys a radial particle physics engine translating fragments outward along randomized vector paths. Enabled OpenGL alpha-blending (`GL_BLEND`) to support proper, smooth alpha-decay fade tracking.

---

## 📂 Project Architecture

```text
neon_geometry/
├── main.cpp                 # Application entry, window framing, loop manager
├── game_engine.h/.cpp       # Unified global structural bridge (SharedState)
├── member1_player.h/.cpp    # Matrix stack composite transform ship engine
├── member2_swarm.h/.cpp     # Target-seeking enemy swarm pooling system
├── member3_lasers.h/.cpp    # Custom low-level algorithmic point-line rasterizer
├── member4_grid.h/.cpp      # Manual 16-element shearing matrix background
├── member5_boss.h/.cpp      # Parametric geometric mesh vertex morpher
└── member6_particles.h/.cpp # Visual hitstop-triggered radial particle burster
```

---

## 🛠️ Setup & Local Installation

### 1. Prerequisites

Ensure your local environment has a working C++ compiler (GCC/MinGW, Clang, or MSVC) and the OpenGL/GLUT development binaries installed.

### 2. Cloning the Project

Open a terminal workspace on your computer and download the repository:

```bash
git clone https://github.com/manodyasasmini/neon_geometry.git
cd neon_geometry
```

---

## 💻 Compilation & Running Guide

### Method A: Code::Blocks IDE (Recommended)

> ⚠️ **CRITICAL:** Do **NOT** create a new project. The folder already contains a pre-configured project workspace tracker (`neon_geometry.cbp`) containing all necessary library linkages.

1. Launch **Code::Blocks**.
2. Navigate to the top menu and select **File** $\rightarrow$ **Open...**
3. Locate your cloned folder and select the **`neon_geometry.cbp`** file.
4. *Optional Troubleshooting:* If the file hierarchy tree on the left sidebar appears blank upon loading, right-click your project root name in the **Management Tree** $\rightarrow$ select **Add Files...**, select all 13 `.cpp` and `.h` files in the main directory, and hit **OK**.
5. Compile and run the engine by pressing **F9** (or clicking **Build and Run**).

### Method B: Terminal CLI Compilation

If compiling strictly via standard command-line tools, you must explicitly declare all individual translation units (`.cpp` files) simultaneously to properly resolve global shared-state linkages across the custom namespaces:

* **Linux / Ubuntu:**
```bash
g++ *.cpp -lGL -lGLU -lglut -o neon_geometry
./neon_geometry
```

* **macOS (Homebrew FreeGLUT framework):**
```bash
g++ *.cpp -framework OpenGL -framework GLUT -o neon_geometry
./neon_geometry
```

* **Windows (MinGW / MSYS2 toolchain environment):**
```bash
g++ *.cpp -lfreeglut -lopengl32 -lglu32 -o neon_geometry
./neon_geometry.exe
```

*If using a specific Code::Blocks MinGW compiler location on Windows, compile directly using:*
```powershell
g++ *.cpp -o neon_geometry.exe -I"C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include" -L"C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/lib" -lfreeglut -lopengl32 -lglu32 -lwinmm -lgdi32
```

---

## 🎮 System Control Mapping

* `W`, `A`, `S`, `D` / `Arrow Keys` — Arena Translation Vector Movement (Case-insensitive & Butter-smooth)
* `Spacebar` — Execute Directional High-Speed Dash
* `Mouse Movement` — Player Core Orientation Rotation Target
* `Left Click` — Execute Custom Algorithmic Laser Discharge Matrix
