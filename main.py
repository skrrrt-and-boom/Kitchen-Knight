from ursina import *
from ursina.prefabs.first_person_controller import FirstPersonController
from stable_baselines3 import PPO
import numpy as np

# 1. Initialize the Engine
app = Ursina()

# Load the trained PPO model (saved as toaster_brain.zip)
model = PPO.load('toaster_brain')

# 2. Visual settings to make it look a bit more "Doom-like" (Retro)
window.title = 'Kitchen Knight 3D'
window.borderless = False
window.fullscreen = False
window.exit_button.visible = False
window.fps_counter.enabled = True

# 3. Create the Environment (The Kitchen Floor)
floor = Entity(
    model='plane',
    texture='white_cube',
    color=color.gray,
    scale=(50, 1, 50),
    collider='box'
)

# 4. Create Walls (Simple loop to create a box)
wall_thickness = 1
wall_height = 5
wall_length = 50

walls = [
    Entity(model='cube', color=color.dark_gray, scale=(wall_length, wall_height, wall_thickness), position=(0, wall_height/2, 25), collider='box'),
    Entity(model='cube', color=color.dark_gray, scale=(wall_length, wall_height, wall_thickness), position=(0, wall_height/2, -25), collider='box'),
    Entity(model='cube', color=color.dark_gray, scale=(wall_thickness, wall_height, wall_length), position=(25, wall_height/2, 0), collider='box'),
    Entity(model='cube', color=color.dark_gray, scale=(wall_thickness, wall_height, wall_length), position=(-25, wall_height/2, 0), collider='box'),
]

# 5. The Player (You, The Kitchen Knight)
player = FirstPersonController(speed=10)
player.cursor.visible = False
player.gravity = 0.5

# Add lighting to the scene
from ursina.shaders import lit_with_shadows_shader
DirectionalLight(y=10, z=-10, shadows=False)
AmbientLight(color=color.rgba(100, 100, 100, 0.4))

# 6. The Enemy (The Evil Toaster - Bright Red)
enemy = Entity(
    model='cube',
    color=color.rgb(255, 0, 0),  # Explicit bright red
    scale=(3, 4, 3),  # Large for visibility
    position=(0, 2, 15),
    collider='box',
    texture='white_cube'  # Use texture to ensure color applies
)

# Grid scale factor: 3D world coordinates to training grid (0-9)
# Training grid was 10x10, 3D world is roughly -25 to 25 (50 units)
GRID_SCALE = 5.0  # Divide 3D coords by 5 to get ~grid coords

def world_to_grid(pos):
    """Convert 3D world position to training grid coordinates (0-9)."""
    # Map from -25..25 to 0..10 range, then clamp to 0-9
    grid_x = np.clip((pos.x + 25) / GRID_SCALE, 0, 9)
    grid_z = np.clip((pos.z + 25) / GRID_SCALE, 0, 9)
    return grid_x, grid_z

def action_to_movement(action):
    """
    Convert model action to movement direction.
    Actions: 0=Stay, 1=Forward(+Z), 2=Back(-Z), 3=Left(-X), 4=Right(+X)
    """
    if action == 0:  # Stay
        return Vec3(0, 0, 0)
    elif action == 1:  # Forward (Up in training = +Z in 3D)
        return Vec3(0, 0, 1)
    elif action == 2:  # Back (Down in training = -Z in 3D)
        return Vec3(0, 0, -1)
    elif action == 3:  # Left (-X)
        return Vec3(-1, 0, 0)
    elif action == 4:  # Right (+X)
        return Vec3(1, 0, 0)
    return Vec3(0, 0, 0)

# Enemy movement speed
ENEMY_SPEED = 3.0

# 7. The Game Loop (Logic)
def update():
    # Get normalized grid positions for observation
    enemy_grid_x, enemy_grid_z = world_to_grid(enemy.position)
    player_grid_x, player_grid_z = world_to_grid(player.position)
    
    # Create observation: [Agent_X, Agent_Y, Target_X, Target_Y]
    # In training: Agent=Toaster(enemy), Target=Player
    # Y in training corresponds to Z in 3D world
    obs = np.array([enemy_grid_x, enemy_grid_z, player_grid_x, player_grid_z], dtype=np.float32)
    
    # Get action from the trained model
    action, _ = model.predict(obs, deterministic=True)
    
    # Convert action to movement direction
    move_dir = action_to_movement(int(action))
    
    # Apply movement to enemy
    enemy.position += move_dir * time.dt * ENEMY_SPEED
    
    # Keep enemy within bounds
    enemy.x = clamp(enemy.x, -24, 24)
    enemy.z = clamp(enemy.z, -24, 24)

# 8. Run the App
app.run()