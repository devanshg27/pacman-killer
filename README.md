# Pacman Killer

## Usage
```sh
mkdir build
cd build
cmake ..
make
./graphics_asgn1
```

## Gameplay
* Jump and land on the **flying balls** to score points(100 points).
* Some flying balls have a **slab** that reflects the ball falling on them and the flying ball is destroyed(50 points are given).
* The flying balls have random heights and speeds.
* The player can move in **water**, and the speed of the ball is decreased due to viscous forces.
* **Magnet** appears randomly on either side and attracts the ball with a constant attractive force towards it.
* Player can use **trampoline** to jump higher.
* Player looses a life on touching randomly appearing **porcupines**(3 lives are given initially).
* The game has an **infinite world**.
* The game has ground with different heights.
* The game statistics are displayed on the screen using a **7-segment display**.

## Controls

`a/Drag mouse left`: Move the ball towards the left direction

`d/Drag mouse right`: Move the ball towards the right direction

`space`: Make the ball jump

`Arrow Keys`: Pan the screen

`Mouse scroll`: Change the zoom of the game

`q`: End the Game

## Levels
The game has 5 levels of progressively increasing difficulty:
1. The player has to reach a score of 2000 points in 80 seconds.
2. The player has to reach a score of 4000 points in 70 seconds, the magnet is introduced in this level to increase the difficulty.
3. The player has to reach a score of 6000 points in 70 seconds, randomly spawning porcupines are introduced in this level to increase the difficulty.
4. The player has to reach a score of 9000 points in 80 seconds.
5. The player has to reach a score of 13000 points in 100 seconds.
