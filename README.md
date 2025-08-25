# Minecraft Playground in C  

This project implements several tasks related to simulating a simplified **Minecraft-like environment** in C.  
The core functionality involves working with 3D chunks of blocks and applying various operations such as placement, filling, rotation, object detection, and compression.  

---

## Task 1: Place a Block  
- Check if the position where the block is to be placed is inside the chunk boundaries.  
- If valid, place the block; otherwise, ignore it.  
- Return the updated chunk.  

---

## Task 2: Place a Cuboid of Blocks  
- Ensure that `(x0, y0, z0)` ≤ `(x1, y1, z1)`. If not, swap the coordinates.  
- Place blocks within the specified cuboid region.  
- Return the updated chunk.  

---

## Task 3: Place a Sphere of Blocks  
- Use the distance formula between two points to check if a block lies inside the given sphere.  
- Traverse the entire chunk, adding blocks if they fall within the radius.  
- Return the updated chunk.  

---

## Task 4: Place a Shell Around Blocks  
- Use 3 direction vectors to simplify traversal around a block.  
- Store all target block positions in a vector.  
- For each target block, add its surrounding shell.  
- Return the updated chunk.  

---

## Task 5: 2D Flood Fill  
- Apply a **recursive 2D flood fill**.  
- If the current block matches the target type, replace it.  
- Continue recursively through the 2D region.  
- Return the updated chunk.  

---

## Task 6: 3D Flood Fill  
- Apply a **recursive 3D flood fill**.  
- If the current block matches the target type, replace it.  
- Continue recursively through the 3D region.  
- Return the updated chunk.  

---

## Task 7: Rotate the Chunk  
- Create a new chunk (`cchunk`) with the rotated dimensions.  
- Copy values using:
```C
cchunk[x][y][z] = chunk[z][y][depth - x - 1]
```
- Return the rotated `cchunk`.  

---

## Task 8: Object and Mega Object Gravity  
- Define:  
- **Object** → a body of connected blocks.  
- **Mega Object** → multiple objects stuck together, regardless of block type.  
- Multiple mega objects may combine into a single mega object.  
- The task is divided into two subtasks:  

### Subtask 1: Mega Object Gravity  
- Traverse the chunk and detect mega objects.  
- For each mega object, store all block positions.  
- A mega object can fall only if it is not already touching the ground.  
- If it can fall, move each block down by one unit along the Y-axis.  

### Subtask 2: Object Gravity  
- Similar to Subtask 1, but applied to **individual objects**.  
- An object falls only if all its blocks either:  
- Have air below them, or  
- Have a block of the same type (inside the object).  

- Return the updated chunk.  

---

## Task 9: Chunk Compression  
- Apply a **greedy run-length encoding** (RLE) approach.  
- Traverse the chunk and store runs of blocks into a vector of unsigned chars (`code`).  
- Handle three encoding cases based on run length `n`:  

### Small Case (`n < 32`)  
- First 2 bits → block type.  
- Remaining bits → length `n`.  

### Medium Case (`32 ≤ n < 4096`)  
- First 2 bits → block type.  
- Third bit → `1` (medium case flag).  
- Next 5 bits → most significant bits of `n`.  
- Store remaining bits of `n` in the next slot.  

### Big Case (`n ≥ 4096`)  
- Until `n < 4096`, repeatedly:  
- Write block type.  
- Set third bit to `1`.  
- Fill the rest with `1`s and continue into the next slot.  
- Once `n < 4096`, use either Small or Medium case.  

- Return the encoded `code`.  

---

## Task 10: Chunk Decompression  
- Medium case and big case are merged for easier decoding.  
- Distinguish between cases using the **third bit**:  
- `0` → Small case.  
- `1` → Medium/Big case.  

### Small Case Decoding  
- Extract block type from the first 2 bits.  
- Place the block the number of times specified in the last 5 bits.  

### Medium/Big Case Decoding  
- Combine bits from the current and next slot to reconstruct `n`.  
- Place the block type `n` times.  
- Repeat until the maximum chunk height is reached.  

---

This concludes the implementation of all tasks in the **Minecraft Playground in C** project.  
