# 📋 BUILD RUNBOOK - HM64 Compilation

## Overview

This document provides the exact steps to compile the Harvest Moon 64 ROM with decompilation changes.

---

## STEP 1: Connect to Docker

```bash
docker run -it --rm --platform linux/amd64 -v "$(pwd)":/hm64 hm64-compiler
```

**Note:** You will be in the project root directory. No need to cd.

---

## STEP 2: Verify baserom exists

```bash
ls -lh baserom.us.z64
```

**Expected result:** ~16MB with recent timestamp

```
-rw-rw-r-- 1 root root 16M Jan  9 21:08 baserom.us.z64
```

---

## STEP 3: Run setup (CRITICAL)

```bash
make setup
```

**What it does:**

- Extracts `ipl3.bin` from the original ROM
- Runs Splat to split the ROM into source files (ASM/C)
- Generates necessary files in `bin/` and `build/`

**Expected result:**

```
Split 16 MB (99.61%) in defined segments
  header:     64 B (0.00%) 1 split, 0 cached
    bin:    15 MB (93.79%) 627 split, 0 cached
  ...
```

---

## STEP 4: Clean previous build (Optional but recommended)

```bash
make clean
```

Removes previously compiled files to ensure a clean compilation.

---

## STEP 5: Compile the ROM with our changes

### Option A: Parallel compilation (RECOMMENDED)

```bash
make -j4
```

### Option B: Normal compilation

```bash
make
```

### Option C: With verbose output (debug)

```bash
make VERBOSE=1
```

**Expected time:** 5-15 minutes depending on system

---

## STEP 6: Verify compiled ROM

```bash
ls -lh hm64.z64
```

**Expected result:** ~16MB with recent timestamp

```
-rwxr-xr-x 1 root root 16M Jan 10 00:24 hm64.z64
```

---

## STEP 7: (OPTIONAL) Compare with original

```bash
make check
```

Compares the compiled ROM with the original to verify they match.

**Expected result:**

```
Binary files hm64.z64 and baserom.us.z64 are identical
OK
```

---

## 🔑 CRITICAL POINTS

### ✅ Requirements

1. **baserom.us.z64 MUST EXIST** in project root (~16MB)
2. **Docker must be running** with `hm64-compiler` image
3. **Read permissions** on baserom.us.z64

### ✅ Correct workflow

```bash
make setup && make -j4
```

Or step by step:

```bash
make setup
make clean
make -j4
```

### ❌ COMMON ERRORS

| Error                          | Cause                       | Solution                               |
| ------------------------------ | --------------------------- | -------------------------------------- |
| `ipl3.bin not found`           | Did not run `make setup`    | Run `make setup` first                 |
| `baserom.us.z64: No such file` | ROM not in project root     | Copy baserom.us.z64 to root directory  |
| `cannot execute binary file`   | Incompatible architecture   | Ensure using `--platform linux/amd64`  |
| `Splat error`                  | Missing Python dependencies | Dockerfile installs them automatically |

---

## 📦 Generated Files

After successful compilation:

```
.
├── hm64.z64                 ← Compiled ROM (16MB)
├── build/
│   ├── src/game/player.o    ← Our changes compiled
│   └── ...
├── bin/
│   └── makerom/ipl3.bin     ← Extracted from baserom
└── asm/                      ← Code split by Splat
```

---

## 🔄 Implementation Details

**File:** `src/game/player.c`

**B Button Functionality:**

- **With item in hand:** If the item is storable (fruits, vegetables, etc.) and there is space in the rucksack, pressing B automatically stores it
- **Without item in hand:** Pressing B equips the selected tool (original behavior)

**Z Button:** Maintains original function (shows item description)

---

## 📝 N64 MEMORY NOTES

- **Rucksack slots:** 9 available spaces (`belongingsSlots[8]`)
- **Item validation:** Only items with `ITEM_RUCKSACK_STORABLE` flag can be stored
- **Space verification:** `addItemToRucksack()` returns `0xFF` if full
- **No memory overwrite:** Uses only pre-existing slots in Player struct

---

## ❓ NEED HELP?

1. Verify you are in Docker: `pwd` should show project root
2. Verify baserom: `ls -lh baserom.us.z64`
3. Verify ipl3.bin: `ls -lh bin/makerom/ipl3.bin`
4. Execute step by step and review expected results

---

**Last updated:** January 9, 2026
**ROM:** Harvest Moon 64 (US Version)
**Status:** 100% Decompiled
