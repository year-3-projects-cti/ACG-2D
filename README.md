# Desert Heist

## Authors
- Prodan Florin Mihai Alexandru
- Dumitru Vlad Andrei

**Faculty of Engineering in Foreign Languages - Group 1231EA**

This project is a 2D OpenGL game using GLFW, GLEW, GLM, and STB. The game involves navigating a character through obstacles, collecting money bags, and avoiding a chasing enforcer. It is a basic example of 2D collision detection, rendering, and simple game mechanics.

## Features:
- **Player movement**: Control a character with WASD keys.
- **Obstacles**: Place and detect collisions with obstacles using mouse input.
- **Money Bags**: Collect money bags scattered across the screen.
- **Enforcer**: A chasing character that attempts to catch the player.
- **Background Image**: A static background rendered using OpenGL shaders.

## Controls:
- **W**: Move character up.
- **S**: Move character down.
- **A**: Move character left.
- **D**: Move character right.
- **Middle Mouse Button**: Place obstacles at the current mouse position.
- **Escape**: Exit the game.

### Algorithms Used

#### 1. **ABBA Algorithm**
The ABBA algorithm helps the player navigate efficiently around obstacles to reach targets (e.g., money bags). It:
- **Analyzes** the environment for obstacles.
- **Builds** a safe path using grid-based logic.
- **Balances** movement with smoothing techniques for natural transitions.
- **Adjusts** dynamically as obstacles or targets change positions.

#### 2. **Enforcer Follow Algorithm**
The Enforcer uses a simple chasing algorithm to track the player:
- Computes the direction vector from its position to the player's.
- Normalizes the vector to maintain constant speed.
- Moves step-by-step toward the player's location, updating every frame.


## Files:
- **Main Game Logic**: The game logic is contained within the main C++ source file `main.cpp`.
- **Shaders**: Vertex and fragment shaders are located in the project directory and are used to render objects and the background.
- **Textures**: Background texture is loaded using STB image.

## How to Run:
1. Clone or download the project.
2. Compile the code (ensure you have OpenGL, GLFW, GLEW, GLM, and STB linked correctly).
3. Run the executable to start the game.
