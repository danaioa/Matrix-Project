# Undercover 2.1 - Matrix Game Project



The code implements the necessary state machine, LCD menu navigation, and the interactive drawing function on the matrix, meeting the core requirements of the project's foundation.

### 1. Menu and Navigation
* **State Structure (`StareAplicatie`):** Implements **Main Menu**, **Brightness Menu**, and **Game Mode** states.
* **LCD Navigation:** Navigation is performed using the joystick's **Y-axis**.
* **LCD Brightness Control:** The "Luminozitate" (Brightness) option allows adjusting the PWM level (`PinLuminaFundal`) and **saving the value to EEPROM** upon confirmation.

### 2. Game Mode (Matrix Drawing)
* **Interactive Cursor:** The joystick controls a cursor (X, Y) on the matrix, which is marked by a permanently lit LED.
* **LED State Toggling:** A **short click** on the button rotates the state of the LED under the cursor through 4 states:
    1.  `Oprit` (**Off**)
    2.  `Fix` (**Fixed**)
    3.  `BlinkLent` (**Slow Blink**) (500ms)
    4.  `BlinkRapid` (**Fast Blink**) (150ms)
* **Exiting Game Mode:** A **long press** (800ms) clears the matrix, displays a **"Smiley" animation**, and returns to the Main Menu.
* **Debounce:** The button uses a **debounce function** (`CitesteButon`) for stable readings.

---

## 3. Components Used

| Component | Description | Wiring |
| :--- | :--- | :--- |
| **Microcontroller** | Arduino Uno/Nano | |
| **LCD Display** | 16x2 (Library: LiquidCrystal) | Pin D4-D7, RS(9), EN(8) |
| **LED Matrix** | 8x8 (Library: LedControl) | DIN(12), CLK(11), LOAD(10) |
| **Joystick** | Analog Sensor | Pin A0 (X), Pin A1 (Y), Pin D2 (Button) |
| **LCD Brightness** | Digital/PWM Pin | Pin D3 (for PWM `analogWrite`) |

---

### 4. Photo of the setup:
<div style="display: flex; justify-content: space-between; gap: 10px; margin-bottom: 10px;">
  <img 
    src="https://github.com/user-attachments/assets/edbdb4a2-e8f2-4640-ae84-3740cb12976b" 
    width="48%" 
    alt="Setup photo 1"
    style="border-radius: 8px; box-shadow: 0 0 8px rgba(0,0,0,0.2);"
  >
  
### 5.Click here for the video demonstrating functionality:
<a href="https://youtu.be/xe3gPDqc564" target="_blank">Watch on YouTube</a>

</details>



<details>
  <summary><b> Checkpoint 2 </b></summary>

This Checkpoint implements a dungeon-crawling style game on an Arduino microcontroller, utilizing an **8x8 LED Matrix** for the main map display and a **16x2 LCD screen** for status, menus, and highscores. The game is controlled using an analog joystick.


  <summary><b> Project Features & Mechanics </b></summary>

The code implements a complete state machine for menu navigation, persistent highscore management, and the core game loop for dungeon exploration.

### 1. Game World and Display
* **Map Size:** The game world is a logical **16x16 grid**, displayed through an 8x8 viewport (camera) on the LED Matrix.
* **Fog of War:** A persistent "Fog of War" (`descoperit[16][16]`) is used, revealing map cells only when the player has been within a 5x5 radius (raza = 2) of them. The fog can be temporarily disabled.
* **Radar Mode:** The ultrasonic sensor enables a temporary **"Radar Mode"** (1 second) if an object is detected closer than 15 cm. In this mode, the entire map is visible.

### 2. Gameplay Mechanics
* **Objective:** Collect all items (`nrItemi`) and then reach the exit cell at `[15, 15]`.
* **Player Movement:** Controlled by the joystick's X and Y axes, with a step interval of 120ms to prevent rapid jumps.
* **Enemy AI:** Enemies move toward the player, with movement speed (`intervalMiscareInamici`) decreasing from 800ms down to 200ms depending on the level.
* **Collision/Damage:** Colliding with an enemy reduces player life count (`vieti`). The player respawns in a corner of the current 8x8 quadrant. If **lives reach 0**, the game is over (`jocActiv = false`).

### 3. Attack Option: The Laser (Chosen Option)
The joystick button implements two functions based on press duration:

* **Laser (Short Press < 500ms):** Fires a beam up to 8 tiles long in the last direction of movement (`lastDirX`, `lastDirY`).
    * Stops when hitting a wall (`harti16[ly][lx] == 1`).
    * Hits an enemy: **+50 score**, enemy is eliminated (`inamici[i].x = -1`).
* **Bomb/Explode (Long Press > 500ms):** This feature is detected but currently results in no action within the main game loop.

### 4. Scoring
| Action | Score Value | Source |
| :--- | :--- | :--- |
| **Collect Item** | $+10 \times \text{Current Level}$ | |
| **Eliminate Enemy (Laser)** | $+50$ | |

### 5. Highscores and EEPROM Layout
* **Highscore Management:** Stores the top 5 scores (`MAX_ENTRIES = 5`).
* **Introduction of Initials:** After a game over, if the score is better than the 3rd place entry (`hs.getScor(2)`), the player enters their 3 initials (`numeTemp`).
* **EEPROM Structure:** The EEPROM stores both highscores and settings:
    * **Address `0`:** Brightness Level (1 byte: `nivelLum`, value $0$ to $4$).
    * **Address `1+`:** Highscore Table. Preceded by `SIGNATURE 0x42` for validation.
        * Each entry (`HighscoreEntry`) consists of 4 bytes for name (`char[4]`) and 2 bytes for score (`uint16_t`).

### 6. LCD Display Updates
The LCD uses custom characters (Full Heart `byte(0)` and Empty Heart `byte(1)`) to display status:
* **Line 1:** Items collected/Total and Lives (e.g., `I:4/6 ♥ ♥ ♡`).
* **Line 2:** Current Level and Score (e.g., `N1 SC:250`).
* **Game Over:** Displays a scrolling "GAME OVER" message.

---

## 7. Components Used

| Component | Description | Wiring | Source |
| :--- | :--- | :--- | :--- |
| **Microcontroller** | Arduino Uno/Nano | | |
| **LCD Display** | 16x2 (Library: LiquidCrystal) | Pin D4-D7, RS(9), EN(8) | |
| **LED Matrix** | 8x8 (Library: LedControl) | DIN(12), CLK(11), LOAD(10) | |
| **Joystick** | Analog Sensor | Pin A0 (X), Pin A1 (Y), Pin D2 (Button) | |
| **LCD Brightness** | Digital/PWM Pin | Pin D3 (for PWM `analogWrite`) | |
| **Buzzer** | Sound Output | Pin A3 | |
| **Ultrasonic Sensor** | Proximity Detection | Initialized via `initUltrasonic()` | |

---


### 8. Photo of the setup:
<div style="display: flex; justify-content: space-between; gap: 10px; margin-bottom: 10px;">
  <img 
    src="https://github.com/user-attachments/assets/d5b0e930-84fc-4bd8-a7c0-62ebb4ac9c84" 
    width="48%" 
    alt="Setup photo 1"
    style="border-radius: 8px; box-shadow: 0 0 8px rgba(0,0,0,0.2);"
  >
  
### 9.Click here for the video demonstrating functionality:
<a href="https://youtu.be/v66D2lMctEg" target="_blank">Watch on YouTube</a>

</details>






## Introduction & Backstory

**Undercover 2.1** is a stealth-based exploration game designed to challenge the player’s **spatial awareness, memory, and strategic thinking**.  
Inspired by classic dungeon crawlers and early stealth titles such as *Metal Gear (NES)*, the game places the player inside a dark, hostile environment where **information is the most valuable resource**.

The project was built around the idea of implementing a **Fog of War** mechanic on a **logical 16×16 map**, while using a **physical 8×8 LED matrix** as a moving viewport.  
This design choice increases tension and difficulty, forcing the player to explore carefully and remember previously revealed areas.

From a technical perspective, the project emphasizes **modularity and clean architecture**, separating:
- game logic,
- display rendering,
- sound and feedback systems.

This approach results in a smoother gameplay experience and a **scalable, professional codebase**.

---

## Game Description

**Undercover** is a stealth-exploration game where you play as a secret agent infiltrating a high-security facility to recover lost intelligence data.

**Logical World & Camera System**

- The game world is a **16×16 grid**.
- The **8×8 LED matrix** functions as a camera that follows the player.
- Only a portion of the world is visible at any time, reinforcing exploration and memorization.


**Dynamic Fog of War**

- The map starts in complete darkness.
- Tiles are revealed only when the player moves close to them.
- Once revealed, tiles remain permanently visible on the explored map.


**Procedural Level Design**

- Each level generates walls and obstacles dynamically.
- This creates maze-like environments and prevents linear navigation paths.


**Enemies & AI**

- Guards patrol the facility using **semi-random AI patterns**.
- Enemies can track the player’s position.
- Difficulty scales progressively:
  - more enemies,
  - increased movement speed.


**Intelligence Items**

- Intelligence data is represented by **blinking pixels**.
- All items must be collected before exiting the level.


**Advanced Sensing Mechanics (Ultrasonic Sensor)**

The ultrasonic sensor is used as a **multi-functional gameplay mechanic**:

**Shield Mode**
- Activated when hand distance is **< 12 cm**
- Creates an energy shield that prevents losing lives on enemy collision

**Radar Mode **
- Activated when hand distance is between **12–30 cm**
- Temporarily reveals all enemies and objectives, bypassing Fog of War
- Each second of usage reduces the score, creating a **risk–reward trade-off**

---

## How to Play

### Movement
- Navigate the map using the **joystick**.

### Objective
- Collect all flashing intelligence items.  
- Once collected, the **Exit Portal at (15, 15)** becomes active and starts blinking.

### Combat
- Press the joystick button briefly to fire a **laser beam** in the current facing direction, neutralizing guards.

### Abilities (Ultrasonic Sensor)
- Shield → hand < 12 cm  
- Radar → hand between 12–30 cm  

### Goal
- Survive **3 levels** and achieve a high score for the leaderboard.

---

## Menu Functionality

The menu is fully navigable using the joystick and includes:

- **Start Game** – Begin the mission at Level 1
- **Settings**
  - Enter Name – Set 3-letter initials for highscores
  - LCD Brightness – Adjustable, saved to EEPROM
  - Matrix Brightness – Adjustable LED intensity with live preview
  - Sound – Toggle audio ON/OFF
  - Reset Highscores – Clears leaderboard data
- **Highscore** – Displays the Top 3 agents
- **How to Play** – Short gameplay instructions
- **About** – Credits, version information, and GitHub link

---

## Components Used

| Component | Quantity |
|---------|----------|
| Arduino Uno | 1 |
| 8×8 LED Matrix (MAX7219) | 1 |
| LCD 1602 | 1 |
| Analog Joystick | 1 |
| HC-SR04 Ultrasonic Sensor | 1 |
| Buzzer | 1 |
| Potentiometer | 1 |
| Resistors, Jumper Wires, Breadboard | as needed |

---

## Photo of the setup:
<img width="476" height="300" alt="image" src="https://github.com/user-attachments/assets/40f2155d-0395-49bc-951d-a28a5aa25129" />


## Click here for the video demonstrating functionality: 
  <a href="https://youtu.be/C5JeNorHvAo">Watch on YouTube</a>




