## cs246-chess

A chess program with both **GUI (X11)** and **CLI (TextDisplay)** output. It supports:
- Human vs Human
- Human vs Computer
- Computer vs Computer

This project was developed as a final assignment for **CS 246 (Object-Oriented Programming), University of Waterloo**.

## Docs
- **UML**: `docs/uml.png` (image), `docs/uml.pdf` (PDF)
- **Design**: `docs/design.pdf`
- **Plan**: `docs/plan.pdf`

Preview:

![UML](docs/uml.png)

## Quickstart (build & run)

### Prerequisites
- C++14 compiler (e.g. `g++`)
- X11 headers & libraries (via XQuartz)

### macOS
- Install **XQuartz** (provides `/opt/X11` on macOS)
- Build & run:

```bash
cd src
make
./chess
```

> Note:
> - This program opens an X11 window. If there is no X server available, it may fail with `Cannot open display`.
> - The current `src/Makefile` is configured for **macOS/XQuartz** (`/opt/X11`). Linux support is not documented here.

## Usage (commands)

### Before a game starts
| Command | Description |
|---|---|
| `game <white> <black>` | Start a new game. Player types: `human`, `computer1`, `computer2`, `computer3`, `computer4`, `computer5` |
| `setup` | Enter custom board setup mode |

Example:

```text
game human computer5
```

### Setup mode
| Command | Description |
|---|---|
| `+ <Piece> <square>` | Place a piece (e.g. `+ K e1`) |
| `- <square>` | Remove a piece (e.g. `- e1`) |
| `= <colour>` | Set the side to move next: `white` or `black` |
| `done` | Exit setup mode (invalid setups are rejected) |

### During a game
| Command | Description |
|---|---|
| `move <from> <to>` | Human move (e.g. `move e2 e4`) |
| `move e7 e8 Q` | Promotion (one of `Q/R/B/N`) |
| `move e1 g1` / `move e1 c1` | Castling via king’s two-square move |
| `move` | Computer move in manual mode (no args) |
| `auto` | Make computer players move continuously without manual `move` input |
| `resign` | Resign the game |
| `takeback` | Undo the last 2 ply (one move by each side) |
| `study` | Toggle global study mode (computers print eval/depth info while thinking) |
| `text on` / `text off` | Toggle CLI(TextDisplay) board output (GUI remains enabled) |

### Quit
- Send EOF (Ctrl+D) to exit; the program prints the final score.

## AI levels
- **Level 1**: random legal move
- **Level 2**: prefers captures and checks
- **Level 3**: prefers avoiding capture, plus captures and checks
- **Level 4**: fixed-depth alpha-beta (minimax + pruning)
- **Level 5**: Level 4 + opening book + time-limited iterative deepening

## Architecture (high-level)
- **Core model**
  - `Board`: game state, piece placement, legal move generation, move history (`moves_played`), and threefold repetition tracking
  - `Square`: coordinates + piece pointer
  - `Piece` + subclasses (`Pawn`, `Knight`, …): piece-specific movement rules
  - `Move`: a move record (from/to, capture/en passant/promotion/castling flags, etc.)
- **Players**
  - `Human`: input parsing/validation
  - `ComputerLevel1~5`: AI players (Level 4/5 are built on `ChessAI`)
- **Rendering (Observer pattern)**
  - `TextDisplay` and `GraphicDisplay` observe `Board` and update on `board.render()` / `notifyObservers()`
  - GUI drawing is done via `Xwindow` (X11)

## Rules / draws implemented
- **Threefold repetition**: `Board` tracks a position key (piece placement + side-to-move + engine-relevant state) and declares a draw on 3 occurrences. AI evaluation treats it as **0 (draw)**.
- **Stalemate** and **insufficient material** are also handled as draw states.

## Opening book (Level 5)
- Opening book lines are defined in `src/openingbook.cc` (`OpeningBook::level5Lines()`).
