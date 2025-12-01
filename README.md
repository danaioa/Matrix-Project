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
