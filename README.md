# ♟️ C++ Chess (c_chess)

A fully playable terminal-based chess game written in C++. This was a fun personal project I built to deepen my understanding of **Object-Oriented Programming (OOP)** concepts like inheritance, polymorphism, and abstract classes.

---

## 🎯 About the Project

I wanted a hands-on way to practice OOP principles, and chess turned out to be a perfect fit — each piece has its own movement logic, the board manages state, and the game orchestrates everything together. Building this from scratch helped me really internalize how to design a clean class hierarchy and use virtual functions effectively.

---

## ✨ Features

- Full chess board rendered in the terminal
- All standard piece movements (Pawn, Rook, Bishop, Knight, Queen, King)
- Check and checkmate detection
- Move validation (prevents illegal moves and self-check)
- Pawn promotion (auto-promotes to Queen)
- Resign option
- Two-player local gameplay (White vs Black, alternating turns)

---

## 🏗️ OOP Design

| Concept | Where It's Used |
|---|---|
| **Abstract base class** | `Piece` with pure virtual `validMove()` |
| **Inheritance** | `Pawn`, `Rook`, `Bishop`, `Knight`, `Queen`, `King` all extend `Piece` |
| **Polymorphism** | `makePiece()` factory returns base `Piece*`; `validMove()` dispatches to the correct subclass |
| **Encapsulation** | `Board` owns the grid; `Game` owns game state and logic |

---

## 🚀 Getting Started

### Prerequisites
- A C++ compiler (g++ recommended)

### Compile & Run

```bash
g++ c_chess.cpp -o chess
./chess
```

---

## 🎮 How to Play

Enter moves as `[source] [destination]` using standard chess notation (column a–h, row 1–8).

```
White > e2 e4
Black > e7 e5
```

Type `exit` at any time to resign.

---

## 📋 Piece Reference

| Symbol | Piece |
|--------|-------|
| `P` / `p` | Pawn (White / Black) |
| `R` / `r` | Rook |
| `B` / `b` | Bishop |
| `N` / `n` | Knight |
| `Q` / `q` | Queen |
| `K` / `k` | King |

Uppercase = White, Lowercase = Black.

---

## 🛠️ Planned Features (for the near future 🫡)

- en passant
- castling
- stalemate detection
- Pawn always promotion with choice

---

## 🛠️ Built With

- C++ (standard library only — `<iostream>`, `<cctype>`, `<vector>`)
- No external dependencies[](url)
