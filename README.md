# Smart Music Playlist Manager

A lightweight, terminal-based music playlist manager written in C — inspired by Spotify but running entirely in your command line.

This project does not use an external music API. Instead, it stores and loads song data from local CSV files, making it easier to manage, test, and understand for a C programming project.

---

## 📋 Features

- **Add Songs** — add new songs into the local music database
- **Delete Songs** — remove songs from the music library or playlist
- **Show All Songs** — display all songs stored in the CSV database
- **Search Songs** — search songs by title, artist, or genre
- **Playlist Management** — manage user playlist using linked list logic
- **Play Queue** — store upcoming songs using queue data structure
- **Listening History** — keep track of played songs using stack/history logic
- **Shuffle Mode** — toggle shuffle on/off
- **Recommendations** — suggest songs based on recent history, artist, or genre
- **CSV Data Storage** — load and save song, playlist, and history data using local CSV files

---

## 🎯 Project Goal

The goal of this project is to create a small music playlist management system using C programming and data structures.

Instead of focusing on real music streaming, this project focuses on how data is stored, searched, played, queued, and recommended inside a terminal-based program.

This project demonstrates:

- File handling in C
- Struct usage
- Array of structs
- Linked list
- Queue
- Stack
- Hash table
- CSV file reading and writing
- Basic recommendation logic
- Modular programming using multiple `.c` and `.h` files

---

## 🗂️ Project Structure

```
smart-music-playlist-manager/
├── src/
│   ├── main.c          # UI, menu loop, program control  [Person 3]
│   ├── song.c          # Song struct definition          [Person 1]
│   ├── playlist.c      # Linked list playlist            [Person 1]
│   ├── queue.c         # Play queue (next song)          [Person 1]
│   ├── stack.c         # History stack (previous song)   [Person 1]
│   ├── hash.c          # Hash table for search           [Person 1]
│   ├── csv.c           # iTunes API / CSV loader         [Person 2]
│   └── recommend.c     # Recommendation algorithm        [Person 2]
├── include/
│   ├── song.h
│   ├── playlist.h
│   ├── csv.h
│   ├── recommend.h
│   └── hash.h
├── data/
│   └── cache.csv       # Local song cache
├── Makefile
└── README.md
```

---

## ⚙️ Requirements

| Tool | Version |
|------|---------|
| GCC  | 7.0+    |
| Make | 3.8+    |
| libcurl | any  |

**Install dependencies (Ubuntu / Debian):**
```bash
sudo apt install build-essential libcurl4-openssl-dev
```

**macOS:**
```bash
xcode-select --install
brew install curl
```

---

## 🚀 Getting Started

**1. Clone the repository**
```bash
git clone https://github.com/tetent888/smart-music-playlist-manager.git
cd smart-music-playlist-manager
```

**2. Build the project**
```bash
make
```

**3. Run**
```bash
./music_manager
```

### During Development (stub mode)
If not all modules are ready yet, build with stub only:
```bash
make stub
./music_manager_stub
```

---

## 🎮 Usage

```
═══════════════════════════════════════
        🎵 Smart Music Playlist Manager
═══════════════════════════════════════

  1. Add Song
  2. Delete Song
  3. Search Song
  4. Play Next Song
  5. Previous Song
  6. Show Playlist
  7. Recommendations
  8. Toggle Shuffle [OFF]
  0. Exit

  Enter your choice:
```

**Now Playing Screen:**
```
  Now Playing : Shape of You - Ed Sheeran
  Duration    : 3:53              Shuffle: OFF

  Next Song   : Beauty and A Beat
  ─────────────────────────────────────────
      [|◄◄]      [ ⏸ ]      [⇄]    [►►|]
     Previous    Pause    Shuffle    Next
```
> Press `[⇄]` to toggle shuffle — shows `Shuffle: ON` when active (random song) or `Shuffle: OFF` to pull from queue in order.

**Example — Adding a song:**
```
── Add Song ──
  Title  : Cruel Summer
  Artist : Taylor Swift
  Genre  : Pop

  ✓ Added: Cruel Summer — Taylor Swift
```

---

## 🔧 Makefile Commands

| Command | Description |
|---------|-------------|
| `make` | Build full project |
| `make stub` | Build main.c only (no dependencies) |
| `make run` | Build and run |
| `make run-stub` | Build stub and run |
| `make clean` | Remove all build files |
| `make info` | Show files being compiled |

---

## 🏗️ System Flow

```
Program starts
        ↓
Load songs from data/songs.csv
        ↓
Store song data into Song struct
        ↓
Insert songs into playlist / linked list / hash table
        ↓
Show main menu
        ↓
User selects an action
        ↓
Possible actions:
    - Add song
    - Delete song
    - Search song
    - Add song to playlist
    - Add song to queue
    - Play next song
    - Show history
    - Show recommendations
        ↓
Save updated data to CSV files
        ↓
Exit program

```

## 🏗️ Detailed System Flow

```
User searches "Taylor Swift"
        ↓
Read data from songs.csv
        ↓
Load each row into Song struct
        ↓
Search by title / artist / genre
        ↓
Display matching songs
        ↓
User selects a song
        ↓
Add song to playlist / queue / history
        ↓
Save updated data to CSV
        ↓
Recommendation system checks history or playlist
        ↓
Recommend songs with similar artist or genre


---

## 👥 Team

| Person | Responsibility | Files |
|--------|---------------|-------|
| Oak| Data Structure Core | playlist.c, queue.c, stack.c,|
| Code| System Logic + API | csv.c, song.c, recommend.c, cache.csv |
| Pea | UI + Control + Integration | main.c, Makefile, README.md,hash.c |

---

## 📄 License

This project is for educational purposes.
