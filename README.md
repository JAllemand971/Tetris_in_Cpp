<h1>Tetris in C++</h1>
A classic **Tetris** implementation in **C++** with a clean architecture (board, pieces, rotation, gravity, line clear).  
The repository’s README notes that **Emscripten** was sometimes used to compile to WebAssembly and run in a browser. :contentReference[oaicite:0]{index=0}

<h2>Demo</h2>

  <table align="center">
  <tr>
    <th>Titre</th>
    <th>Description</th>
  </tr>
  <tr>
    <td>Tetris</td>
    <td>

https://github.com/user-attachments/assets/56e1b272-2524-49d9-aaca-9246fe04b89d
      
  </tr>
  </table>

---

## What it does

- Maintains a **10×20 grid** (standard Tetris well) and spawns tetrominoes (I, O, T, S, Z, J, L).
- Applies **gravity**, **collision**, **locking**, and **line clear** rules.
- Handles **piece rotation** and **horizontal movement** with keyboard.
- Tracks **score** and **speed** progression (level) as lines are cleared.  
- Includes a **demo recording** in the repo (see below). :contentReference[oaicite:1]{index=1}

> A `Tetris/` folder exists in the repository; see the code there for exact file names and key bindings. :contentReference[oaicite:2]{index=2}

---

## How it works (high-level pipeline)

1. **State model**
   - **Board**: a 2D array / vector representing occupied cells.
   - **Tetromino**: shape definition (relative blocks), current position `(x, y)`, and rotation index.
   - **Bag/Spawner**: picks next tetromino (e.g., simple random or 7-bag).

2. **Tick & input**
   - On each tick, gravity attempts to move the active piece down.
   - Keyboard input updates horizontal movement and rotation (if legal).

3. **Collision & lock**
   - Movement/rotation is accepted only if it **doesn’t collide** (inside bounds and not overlapping filled cells).
   - If moving down is no longer possible, the piece **locks** into the board.

4. **Line clear & score**
   - Full lines are removed; everything above is shifted down.
   - Score and level update (e.g., more points for multi-line clears).

5. **Game over**
   - If a new piece collides at spawn, the game ends.

> Rotation can follow a simple matrix rotation or SRS-like wall kicks. Check your `Tetris/` sources for the exact policy.

---

## Repository structure


