"""
Kitchen Knight AI Training Script
==================================
Trains a neural network to chase the player using PPO (Proximal Policy Optimization).
Includes real-time visualization and video recording.
"""

import gymnasium as gym
from gymnasium import spaces
import numpy as np
from stable_baselines3 import PPO
from stable_baselines3.common.callbacks import BaseCallback
import pygame
import cv2
import os

# ============= CONFIGURATION =============
GRID_SIZE = 10
CELL_SIZE = 60  # Pixels per cell
WINDOW_SIZE = GRID_SIZE * CELL_SIZE
FPS = 30
TRAINING_TIMESTEPS = 10000
RECORD_VIDEO = True
VIDEO_FILENAME = "training_video.mp4"


class KitchenEnv(gym.Env):
    """
    A 2D Grid Environment where the Agent (Toaster) learns to catch the Target (Player).
    Fixed boundary bugs and added Pygame visualization.
    """
    metadata = {"render_modes": ["human", "rgb_array"], "render_fps": FPS}
    
    def __init__(self, render_mode=None):
        super(KitchenEnv, self).__init__()
        self.grid_size = GRID_SIZE
        self.render_mode = render_mode
        
        # Action Space: 0=Stay, 1=Up, 2=Down, 3=Left, 4=Right
        self.action_space = spaces.Discrete(5)
        
        # Observation Space: [Agent_X, Agent_Y, Target_X, Target_Y]
        # FIX: high should be grid_size - 1 (valid indices are 0 to 9)
        self.observation_space = spaces.Box(
            low=0, 
            high=self.grid_size - 1, 
            shape=(4,), 
            dtype=np.float32
        )
        
        # Pygame setup (lazy initialization)
        self.screen = None
        self.clock = None
        self.font = None
        
        # State
        self.agent_pos = None
        self.target_pos = None
        self.steps_left = 50

    def reset(self, seed=None, options=None):
        super().reset(seed=seed)
        
        # Spawn Agent and Target at random spots
        self.agent_pos = np.random.randint(0, self.grid_size, size=2).astype(np.float32)
        self.target_pos = np.random.randint(0, self.grid_size, size=2).astype(np.float32)
        
        # Ensure they don't spawn on top of each other
        while np.array_equal(self.agent_pos, self.target_pos):
            self.target_pos = np.random.randint(0, self.grid_size, size=2).astype(np.float32)
            
        self.steps_left = 50
        return self._get_obs(), {}

    def _get_obs(self):
        return np.concatenate([self.agent_pos, self.target_pos]).astype(np.float32)

    def step(self, action):
        # Calculate distance BEFORE moving
        dist_before = np.linalg.norm(self.agent_pos - self.target_pos)

        # Apply Movement (FIX: use grid_size - 1 for upper bound)
        if action == 1:  # Up
            self.agent_pos[1] = min(self.agent_pos[1] + 1, self.grid_size - 1)
        elif action == 2:  # Down
            self.agent_pos[1] = max(self.agent_pos[1] - 1, 0)
        elif action == 3:  # Left
            self.agent_pos[0] = max(self.agent_pos[0] - 1, 0)
        elif action == 4:  # Right
            self.agent_pos[0] = min(self.agent_pos[0] + 1, self.grid_size - 1)
        
        # Calculate distance AFTER moving
        dist_after = np.linalg.norm(self.agent_pos - self.target_pos)
        
        # Reward Logic
        reward = 0.0
        terminated = False
        
        # Did we catch the player?
        if np.array_equal(self.agent_pos.astype(int), self.target_pos.astype(int)):
            reward = 10.0  # BIG REWARD
            terminated = True
        else:
            # Shaping Reward: Reward for getting closer, penalty for moving away
            reward = (dist_before - dist_after) * 2.0
            # Small penalty for existing (encourages speed)
            reward -= 0.1

        self.steps_left -= 1
        truncated = self.steps_left <= 0
        
        return self._get_obs(), reward, terminated, truncated, {}

    def render(self):
        if self.render_mode is None:
            return None
            
        # Initialize Pygame on first render
        if self.screen is None:
            pygame.init()
            if self.render_mode == "human":
                pygame.display.set_caption("Kitchen Knight AI Training")
                self.screen = pygame.display.set_mode((WINDOW_SIZE, WINDOW_SIZE))
            else:
                self.screen = pygame.Surface((WINDOW_SIZE, WINDOW_SIZE))
            self.clock = pygame.time.Clock()
            self.font = pygame.font.Font(None, 36)
        
        # Fill background
        self.screen.fill((40, 40, 40))  # Dark gray
        
        # Draw grid lines
        for i in range(self.grid_size + 1):
            pygame.draw.line(self.screen, (80, 80, 80), 
                           (i * CELL_SIZE, 0), (i * CELL_SIZE, WINDOW_SIZE))
            pygame.draw.line(self.screen, (80, 80, 80), 
                           (0, i * CELL_SIZE), (WINDOW_SIZE, i * CELL_SIZE))
        
        # Draw Target (Player) - Red
        target_rect = pygame.Rect(
            int(self.target_pos[0]) * CELL_SIZE + 5,
            (self.grid_size - 1 - int(self.target_pos[1])) * CELL_SIZE + 5,
            CELL_SIZE - 10,
            CELL_SIZE - 10
        )
        pygame.draw.rect(self.screen, (220, 50, 50), target_rect)
        
        # Draw Agent (Toaster) - Green
        agent_rect = pygame.Rect(
            int(self.agent_pos[0]) * CELL_SIZE + 5,
            (self.grid_size - 1 - int(self.agent_pos[1])) * CELL_SIZE + 5,
            CELL_SIZE - 10,
            CELL_SIZE - 10
        )
        pygame.draw.rect(self.screen, (50, 220, 50), agent_rect)
        
        # Draw labels
        agent_label = self.font.render("A", True, (255, 255, 255))
        target_label = self.font.render("P", True, (255, 255, 255))
        self.screen.blit(agent_label, (agent_rect.centerx - 8, agent_rect.centery - 10))
        self.screen.blit(target_label, (target_rect.centerx - 8, target_rect.centery - 10))
        
        # Draw info
        info_text = self.font.render(f"Steps: {self.steps_left}", True, (255, 255, 255))
        self.screen.blit(info_text, (10, 10))
        
        if self.render_mode == "human":
            pygame.display.flip()
            self.clock.tick(FPS)
            
            # Handle pygame events
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    pygame.quit()
        
        # Return RGB array for video recording
        return np.transpose(
            np.array(pygame.surfarray.pixels3d(self.screen)), 
            axes=(1, 0, 2)
        )

    def close(self):
        if self.screen is not None:
            pygame.quit()
            self.screen = None


class RenderCallback(BaseCallback):
    """
    Callback to render the environment during training (every N steps).
    """
    def __init__(self, render_freq=500, verbose=0):
        super(RenderCallback, self).__init__(verbose)
        self.render_freq = render_freq

    def _on_step(self) -> bool:
        if self.n_calls % self.render_freq == 0:
            self.training_env.render()
        return True


def record_test_episode(model, env, video_filename, num_episodes=3):
    """
    Record test episodes to a video file.
    """
    print(f"🎬 Recording {num_episodes} test episodes to '{video_filename}'...")
    
    # Setup video writer
    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    out = cv2.VideoWriter(video_filename, fourcc, FPS, (WINDOW_SIZE, WINDOW_SIZE))
    
    for episode in range(num_episodes):
        obs, _ = env.reset()
        done = False
        step_count = 0
        
        while not done:
            # Render and capture frame
            frame = env.render()
            if frame is not None:
                # Convert RGB to BGR for OpenCV
                frame_bgr = cv2.cvtColor(frame, cv2.COLOR_RGB2BGR)
                out.write(frame_bgr)
            
            # Get action from trained model
            action, _ = model.predict(obs, deterministic=True)
            obs, reward, terminated, truncated, _ = env.step(action)
            done = terminated or truncated
            step_count += 1
            
            # Add delay for human-visible playback later
            pygame.time.wait(50)
        
        print(f"  Episode {episode + 1}: {'🏆 CAUGHT!' if terminated else '⏱️ Timeout'} in {step_count} steps")
    
    out.release()
    print(f"✅ Video saved to '{video_filename}'")


def main():
    print("=" * 50)
    print("⚔️  KITCHEN KNIGHT - AI TRAINING")
    print("=" * 50)
    
    # 1. Create the Environment
    print("\n📦 Creating environment...")
    env = KitchenEnv(render_mode="rgb_array")
    
    # 2. Create the Brain (PPO Model)
    print("🧠 Initializing PPO neural network...")
    print("   Architecture: 4 inputs → [64, 64] hidden → 5 outputs")
    model = PPO(
        "MlpPolicy", 
        env, 
        verbose=1,
        learning_rate=0.0003,
        n_steps=2048,
        batch_size=64,
        n_epochs=10,
        gamma=0.99,
        gae_lambda=0.95,
        clip_range=0.2,
    )
    
    # 3. Train the Brain
    print(f"\n🏋️ Training for {TRAINING_TIMESTEPS} timesteps...")
    print("   (This takes about 10-30 seconds)")
    model.learn(total_timesteps=TRAINING_TIMESTEPS)
    
    # 4. Save the Brain
    model.save("toaster_brain")
    print("\n💾 Brain saved as 'toaster_brain.zip'")
    
    # 5. Record test episodes
    if RECORD_VIDEO:
        test_env = KitchenEnv(render_mode="rgb_array")
        record_test_episode(model, test_env, VIDEO_FILENAME, num_episodes=5)
        test_env.close()
    
    # 6. Interactive test (with visualization)
    print("\n🧪 Running interactive test (watch the window!)...")
    vis_env = KitchenEnv(render_mode="human")
    obs, _ = vis_env.reset()
    
    for i in range(50):
        vis_env.render()
        action, _ = model.predict(obs, deterministic=True)
        obs, reward, terminated, truncated, _ = vis_env.step(action)
        print(f"Step {i+1}: Agent at {obs[:2].astype(int)} → Target at {obs[2:].astype(int)}")
        
        if terminated:
            print("🏆 CAUGHT THE PLAYER!")
            pygame.time.wait(1000)
            break
        if truncated:
            print("⏱️ Ran out of time!")
            break
    
    vis_env.close()
    print("\n✅ Training complete!")


if __name__ == "__main__":
    main()
