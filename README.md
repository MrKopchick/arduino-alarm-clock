# Arduino Alarm Clock

An Arduino-based(Arduino Uno) digital clock with alarm functionality, displayed via an I2C LCD screen. Time can be manually adjusted, and an alarm can be set with a buzzer alert.

## Features

- Real-time clock with hour, minute, and second display
- Adjustable current time (hours and minutes)
- Configurable alarm (hours and minutes)
- Alarm indicator with ON/OFF status on LCD
- Buzzer sound when alarm triggers
- Debounced button controls for mode switching and value editing
- Mode-based editing system for intuitive configuration
- Leading-zero formatting for consistent time display (e.g., `07:03:09`)

## Components Used

- Arduino (Uno/Nano/etc.)
- I2C 16x2 LCD display (address `0x27`)
- 3 Push buttons (Mode, Up, Down)
- Passive buzzer
- Resistors (if needed for debouncing or pull-down setup)

## Pin Configuration

| Component    | Arduino Pin |
|--------------|-------------|
| Mode Button  | D5          |
| Up Button    | D6          |
| Down Button  | D7          |
| Buzzer       | D8          |
| I2C LCD      | A4 (SDA), A5 (SCL) |

## Button Modes

- **Mode Button (D5)** cycles through:
  - `0`: Normal view (no edit)
  - `1`: Set current hour
  - `2`: Set current minute
  - `3`: Set alarm hour
  - `4`: Set alarm minute

- **Up/Down Buttons (D6/D7)** increment/decrement the current mode’s value

## Alarm Behavior

- If alarm is ON and time matches the set alarm time (hour + minute), the buzzer sounds for 5 seconds
- Alarm auto-disables after triggering once

## Notes

- Uses internal `millis()` timer for clock updates (not RTC)
- Alarm checks every second for precise triggering
