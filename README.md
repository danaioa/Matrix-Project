# MATRIX PROJECT


This project implements a menu system and an interactive interface on an **8x8 LED Matrix** and a **16x2 LCD screen**, controlled by a **joystick**.

<details>
  <summary><b> Checkpoint 1 </b></summary>



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


