# Neon Geometry: Protocol

A minimalistic, cinematic tech-aesthetic 2D survival arena shooter built from scratch in C++ using OpenGL and the GLUT utility toolkit. This repository showcases structural computer graphics fundamentals distributed across a 6-member engineering team.

---

## 👥 6-Member Responsibility Breakdown

To avoid workflow conflicts, the engine architecture isolates individual math pipelines into standalone modular codebases:

- **Member 1 (Player Core):** Manages compound matrix operations (`glTranslate`, `glRotate`, `glScale`) driving the primary entity and local dynamic thruster oscillations.
- **Member 2 (The Swarm):** Implements automated targeted vector pathways enabling AI groups to seek and translate smoothly toward the player's real-time coordinate state.
- **Member 3 (Laser Projectiles):** Bypasses basic hardware line-loops to execute an academic **Digital Differential Analyzer (DDA) / Bresenham's rasterization algorithm** plotting high-speed bullet points manually.
- **Member 4 (The Pulse Grid):** Owns background projection using customized Row-Major 4x4 Shearing Matrices to map depth tilt along with a rhythmic heartbeat scaling wrapper.
- **Member 5 (The Anomaly Boss):** Orchestrates procedural parametric equations (`sin`/`cos`) deforming geometry vertices dynamically over time to create an unstable morphing boss variant.
- **Member 6 (Particle Explosions):** Deploys a radial particle physics engine translating fragments outward along randomized vector paths while handling automatic alpha-decay fade tracking.

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

Here is the fully formatted Markdown code for your setup, compilation guide, and controls. You can paste this directly into your `README.md` or any other Markdown document.

````markdown
## 🛠️ Setup & Local Installation

### 1. Prerequisites

Ensure your environment has a C++ compiler (GCC/MinGW, Clang, or MSVC) and the OpenGL/GLUT binaries installed.

### 2. Cloning/Downloading the Project

```bash
git clone [https://github.com/YOUR_USERNAME/neon_geometry.git](https://github.com/YOUR_USERNAME/neon_geometry.git)
cd neon_geometry
```
````

---

## 💻 Compilation & Running Guide

### Method A: Code::Blocks IDE (Recommended)

1. Launch **Code::Blocks** and open your current project workspace.
2. Right-click your project name in the left-hand **Management Tree** $\rightarrow$ **Add Files...**
3. Select **all 13 files** (`.cpp` and `.h`) in the directory.
4. When prompted with the target checkbox modal, verify both **Debug** and **Release** are checked, then click **OK**.
5. Click **Build and Run** (or press **F9**).

### Method B: Terminal CLI Compilation

If compiling strictly via terminal, you must include all translation units (`.cpp` files) simultaneously to link the namespaces:

- **Linux / Ubuntu:**

```bash
g++ *.cpp -lGL -lGLU -lglut -o neon_geometry
./neon_geometry

```

- **macOS (Homebrew GLUT):**

```bash
g++ *.cpp -framework OpenGL -framework GLUT -o neon_geometry
./neon_geometry

```

- **Windows (MinGW/MSYS2):**

```bash
g++ *.cpp -lfreeglut -lopengl32 -lglu32 -o neon_geometry
./neon_geometry.exe

```

---

## 🎮 System Control Mapping

- `W`, `A`, `S`, `D` — Arena Translation Vector Movement
- `Mouse Movement` — Player Core Orientation Rotation Target
- `Left Click` — Execute Custom Algorithmic Laser Discharge Matrix
