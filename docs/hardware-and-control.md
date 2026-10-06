# Hardware and control notes

## Main controller

The main firmware is built around an STM32F103RC. TIM4 channels 3 and 4 generate the two motor PWM outputs in PWM2 mode. A signed command in the range `[-100, 100]` selects both direction and duty-cycle magnitude, allowing all higher-level behaviours to use the same motor API.

TIM5 channel 1 produces a 50 Hz servo signal. The sensor platform is positioned at 90 degrees during startup so every run begins with a repeatable mechanical orientation.

## Sensor conventions

| Sensor | Left | Right | Active interpretation |
| --- | --- | --- | --- |
| Line following | PB0 | PA7 | Black line = 1, light floor = 0 |
| Obstacle detection | PA8 | PB1 | Obstacle = 0, clear = 1 |

The obstacle input must remain active for 30 ms before it changes the autonomous behaviour. This filters brief reflections and electrical noise.

## Control behaviours

### Line following

| Left sensor | Right sensor | Action |
| --- | --- | --- |
| White | White | Forward |
| Black | White | Correct left |
| White | Black | Correct right |
| Black | Black | Forward/default fallback |

PC1 adds a digital ambient-light state to the tracking controller. When active, the straight-line speed changes from 60 to 40 and the buzzer indicates the low-light state. Steering commands add a speed margin to keep correction authority at the lower base speed.

### Obstacle avoidance

| Left detector | Right detector | Action |
| --- | --- | --- |
| Clear | Clear | Forward; alarm off |
| Clear | Blocked | Turn left |
| Blocked | Clear | Turn right |
| Blocked | Blocked | Alarm, brake, reverse, brake, spin |

### Infrared remote

The PA1 input uses rising- and falling-edge EXTI events. The decoder measures NEC pulse widths with the DWT cycle counter, assembles a 32-bit frame, and exposes the command byte to the main loop. Interrupt code only captures and decodes the signal; motor actions stay in the main loop to avoid current spikes and jitter caused by executing motion routines in an ISR.

After a valid key is received, autonomous tracking and avoidance are suppressed for 800 ms. If autonomous mode is not enabled once that window expires, the vehicle brakes.

## Auxiliary indicator board

The STM32F103C8 board reads PA4 as its enable/control input. When active, PA1, PA2, and PA3 are reset. Otherwise, the firmware advances through the three outputs at 200 ms intervals to create a repeating indicator sequence.

## Bring-up checklist

1. Verify regulated 5 V and 3.3 V rails before connecting the MCU and sensors.
2. Test each motor independently with the chassis lifted; reverse the direction mapping if wheel installation differs.
3. Adjust both line sensors so their thresholds switch consistently over the same black tape and light floor.
4. Adjust obstacle-detector range for the intended environment; highly reflective or dark surfaces may require retuning.
5. Confirm the remote produces stable NEC command values before enabling motor power.
6. Test the light-state input and servo position independently, then enable the full scheduler.
