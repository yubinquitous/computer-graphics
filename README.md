# 3D Virtual Human Modeling & Visualization
<img width="1512" alt="스크린샷 2024-06-19 오전 11 44 27" src="https://github.com/yubinquitous/computer-graphics/assets/66323295/c0dab6c7-4e46-428f-886c-1b083e18b252">

## About The Project
This project focuses on 3D graphics programming, specifically on the creation of a realistic 3D human model using OpenGL. The goal is to enhance our understanding and skills in 3D modeling and visualization through practical application.

### Key Features
- **Realistic 3D Human Modeling**: Models different body parts using OpenGL primitives, with detailed texture mapping for enhanced realism.
- **Interactive Features**: Includes interactive functionalities such as object rotation using mouse dragging and zooming in/out with keyboard controls ('a' to zoom in, 'z' to zoom out).
- **Real-Time Rendering**: Leverages OpenGL for dynamic rendering, facilitating an interactive 3D environment.
- **Team Collaboration**: Focuses on efficient project management and documentation in collaboration with team members.

## Platform Compatibility
**Note**: This project is specifically designed for the Windows operating system due to its dependency on the `windows.h` header file. Users on other platforms may need to modify or replace certain parts of the code to ensure compatibility.

## Getting Started
### Prerequisites
- A Windows operating system

### Installation
Clone the repository:
```bash
git clone https://github.com/yubinquitous/computer-graphics.git
```

Navigate to the project directory:
```bash
cd computer-graphics/gl_assignment/gl_assignment/
```

### Running the Program
Compile the source code using a C++ compiler that supports OpenGL:
```bash
g++ -o VirtualHuman human.cpp -lGL -lGLU -lglut
```

Run the compiled program:
```bash
./VirtualHuman
```

## Usage
- **Rotate**: Click and drag the mouse to rotate the model.
- **Zoom**: Press 'a' to zoom in and 'z' to zoom out.
- **Exit**: Press the ESC key to exit the application.

## Contributors
- [Choi Yubin](https://github.com/yubinquitous)
- [Lee Sihah](https://github.com/leesiha)
