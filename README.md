# vga_arcade
1TE663 project

One Arduino controls the VGA output, controls the slave Arduino to play music/sfx, reads Wii-controller and handles the game logic.

The code produces 2-bit color output, and in this project we have used the following colors:
![2-bit-colors](/Arcade-Main/res/ColorTable2bit.png)

| | R | G | B|
| --- | --- | --- | --- |
|00: Black|	0|	0|	0|
|01: Cyan|	0|	1|	1|
|10: Red|	1|	0|	0|
|11: White|	1|	1|	1|


The project milestones:
- [x] Get a stable VGA image on a TFT-screen using PWM.
- [x] Use an analog potentiometer to control the game paddle.
- [x] Moving bouncing ball in a playable game.
- [ ] Use a Wii-nunchuk (I2C) to control the game paddle.
- [ ] Create advanced music and sfx using a secondary Arduino.
- [ ] Create an simple AI that can play against a human player.
- [ ] Create a 3D-printed box for the hardware.
- [ ] Create the project report (=> 10 pages).
- [ ] Create a project presentation (Google Presentation).

The actual project progress are marked below:
- [x] Code adjustment for C++ syntax.
- [x] Prototype hardware constructed and assembled.
- [x] Both ordinary buttons are now active low with internal pull-ups.
- [ ] Blue VGA-line connected to green. TODO: Improve VGAX to use more colors.
- [ ] Wii-nunchuk routine will be created.
- [ ] SID Music and sound effects using an slave Atmel 328P. :slightly_smiling_face:
