#include <iostream>
#include <cctype>
#include <vector>
using namespace std;

// ---------------- PIECE BASE CLASS ----------------
class Piece {
public:
    char symbol;

    Piece(char s) : symbol(s) {}
    virtual ~Piece() {}

    bool isWhite() { return isupper(symbol); }
    bool isBlack() { return islower(symbol); }

    virtual bool validMove(int sr, int sc, int dr, int dc, char board[8][8]) = 0;
};

// ---------------- UTILS ----------------
bool isEmpty(char p) { return p == '.'; }
bool isWhiteP(char p) { return isupper(p); }
bool isBlackP(char p) { return islower(p); }

bool clearPath(int sr, int sc, int dr, int dc, char board[8][8]) {
    int rStep = (dr > sr) ? 1 : (dr < sr) ? -1 : 0;
    int cStep = (dc > sc) ? 1 : (dc < sc) ? -1 : 0;

    sr += rStep;
    sc += cStep;

    while (sr != dr || sc != dc) {
        if (!isEmpty(board[sr][sc])) return false;
        sr += rStep;
        sc += cStep;
    }
    return true;
}

// ---------------- PAWN ----------------
class Pawn : public Piece {
public:
    Pawn(char s) : Piece(s) {}

    bool validMove(int sr, int sc, int dr, int dc, char board[8][8]) override {
        int dir   = isWhite() ? -1 : 1;
        int start = isWhite() ?  6 : 1;

        if (sc == dc && isEmpty(board[dr][dc])) {
            if (dr == sr + dir) return true;
            if (sr == start && dr == sr + 2 * dir &&
                isEmpty(board[sr + dir][sc])) return true;
        }

        if (abs(dc - sc) == 1 && dr == sr + dir &&
            !isEmpty(board[dr][dc]) &&
            isWhite() != isWhiteP(board[dr][dc]))
            return true;

        return false;
    }
};

// ---------------- ROOK ----------------
class Rook : public Piece {
public:
    Rook(char s) : Piece(s) {}

    bool validMove(int sr, int sc, int dr, int dc, char board[8][8]) override {
        return (sr == dr || sc == dc) && clearPath(sr, sc, dr, dc, board);
    }
};

// ---------------- BISHOP ----------------
class Bishop : public Piece {
public:
    Bishop(char s) : Piece(s) {}

    bool validMove(int sr, int sc, int dr, int dc, char board[8][8]) override {
        return abs(sr - dr) == abs(sc - dc) && clearPath(sr, sc, dr, dc, board);
    }
};

// ---------------- KNIGHT ----------------
class Knight : public Piece {
public:
    Knight(char s) : Piece(s) {}

    bool validMove(int sr, int sc, int dr, int dc, char board[8][8]) override {
        int r = abs(sr - dr), c = abs(sc - dc);
        return (r == 2 && c == 1) || (r == 1 && c == 2);
    }
};

// ---------------- QUEEN ----------------
class Queen : public Piece {
public:
    Queen(char s) : Piece(s) {}

    bool validMove(int sr, int sc, int dr, int dc, char board[8][8]) override {
        bool straight  = (sr == dr || sc == dc) && clearPath(sr, sc, dr, dc, board);
        bool diagonal  = abs(sr - dr) == abs(sc - dc) && clearPath(sr, sc, dr, dc, board);
        return straight || diagonal;
    }
};

// ---------------- KING ----------------
class King : public Piece {
public:
    King(char s) : Piece(s) {}

    bool validMove(int sr, int sc, int dr, int dc, char board[8][8]) override {
        return abs(sr - dr) <= 1 && abs(sc - dc) <= 1;
    }
};

// ---------------- PIECE FACTORY ----------------
Piece* makePiece(char symbol) {
    switch (tolower(symbol)) {
        case 'p': return new Pawn(symbol);
        case 'r': return new Rook(symbol);
        case 'b': return new Bishop(symbol);
        case 'n': return new Knight(symbol);
        case 'q': return new Queen(symbol);
        case 'k': return new King(symbol);
    }
    return nullptr;
}

// ---------------- BOARD CLASS ----------------
class Board {
public:
    char grid[8][8];

    Board() {
        char init[8][8] = {
            {'r','n','b','q','k','b','n','r'},
            {'p','p','p','p','p','p','p','p'},
            {'.','.','.','.','.','.','.','.'},
            {'.','.','.','.','.','.','.','.'},
            {'.','.','.','.','.','.','.','.'},
            {'.','.','.','.','.','.','.','.'},
            {'P','P','P','P','P','P','P','P'},
            {'R','N','B','Q','K','B','N','R'}
        };
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                grid[i][j] = init[i][j];
    }

    void print() {
        cout << "\n  a b c d e f g h\n";
        for (int i = 0; i < 8; i++) {
            cout << 8 - i << " ";
            for (int j = 0; j < 8; j++)
                cout << grid[i][j] << " ";
            cout << 8 - i << endl;
        }
        cout << "  a b c d e f g h\n\n";
    }

    void pawnPromotion(int r, int c) {
        if (grid[r][c] == 'P' && r == 0) grid[r][c] = 'Q';
        if (grid[r][c] == 'p' && r == 7) grid[r][c] = 'q';
    }
};

// ---------------- GAME CLASS ----------------
class Game {
    Board board;
    bool  whiteTurn;

    // ---------------- VALID MOVE CHECK ----------------
    bool isValidMove(int sr, int sc, int dr, int dc) {
        char p = board.grid[sr][sc];
        char t = board.grid[dr][dc];

        if (isEmpty(p)) return false;
        if (!isEmpty(t) && isWhiteP(p) == isWhiteP(t)) return false;

        Piece* piece = makePiece(p);
        if (!piece) return false;

        bool result = piece->validMove(sr, sc, dr, dc, board.grid);
        delete piece;
        return result;
    }

    // ---------------- CHECK ----------------
    bool isKingInCheck(bool white) {
        int kr = -1, kc = -1;
        char king = white ? 'K' : 'k';

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                if (board.grid[i][j] == king) { kr = i; kc = j; }

        if (kr == -1) return false;

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++) {
                char p = board.grid[i][j];
                if (isEmpty(p) || isWhiteP(p) == white) continue;
                if (isValidMove(i, j, kr, kc)) return true;
            }
        return false;
    }

    // ---------------- CHECKMATE ----------------
    bool isCheckmate(bool white) {
        if (!isKingInCheck(white)) return false;

        for (int sr = 0; sr < 8; sr++) {
            for (int sc = 0; sc < 8; sc++) {
                char p = board.grid[sr][sc];
                if (isEmpty(p) || isWhiteP(p) != white) continue;

                for (int dr = 0; dr < 8; dr++) {
                    for (int dc = 0; dc < 8; dc++) {
                        if (!isValidMove(sr, sc, dr, dc)) continue;

                        char captured       = board.grid[dr][dc];
                        board.grid[dr][dc]  = p;
                        board.grid[sr][sc]  = '.';

                        bool stillCheck = isKingInCheck(white);

                        board.grid[sr][sc]  = p;
                        board.grid[dr][dc]  = captured;

                        if (!stillCheck) return false;
                    }
                }
            }
        }
        return true;
    }

public:
    Game() : whiteTurn(true) {}

    // ---------------- MAIN LOOP ----------------
    void run() {
        string from, to;
        cout<<"For every move enter piece location and target location, ex: e2 e4"<<endl<<endl;
        cout<<"Type exit to resign"<<endl<<endl;
        cout<<"WELCOME TO ";
         cout << "\n";
        cout << "  ██████╗██╗  ██╗███████╗███████╗███████╗\n";
        cout << " ██╔════╝██║  ██║██╔════╝██╔════╝██╔════╝\n";
        cout << " ██║     ███████║█████╗  ███████╗███████╗\n";
        cout << " ██║     ██╔══██║██╔══╝  ╚════██║╚════██║\n";
        cout << " ╚██████╗██║  ██║███████╗███████║███████║\n";
        cout << "  ╚═════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n";
        cout << "\n";
        while (true) {
            board.print();

            if (isCheckmate(whiteTurn)) {
                cout << "♚ CHECKMATE! "
                     << (whiteTurn ? "Black" : "White")
                     << " wins !!!!!\n";
                break;
            }

            if (isKingInCheck(whiteTurn))
                cout << "CHECK!\n";

            cout << (whiteTurn ? "White" : "Black")
                 << " > ";
            cin >> from;
            if (from == "exit")
                {
                 cout<<(whiteTurn ? "White" : "Black") << " resigned"<<endl;
                 cout<<(whiteTurn ? "Black" : "White") << " wins !!!"<<endl;
                 break;}
            cin >> to;

            int sc = from[0] - 'a';
            int sr = 8 - (from[1] - '0');
            int dc = to[0] - 'a';
            int dr = 8 - (to[1] - '0');

            char piece = board.grid[sr][sc];

            if ((whiteTurn && !isWhiteP(piece)) ||
                (!whiteTurn && !isBlackP(piece))) {
                cout << "Wrong piece\n";
                continue;
            }

            if (!isValidMove(sr, sc, dr, dc)) {
                cout << "Illegal move\n";
                continue;
            }

            char captured       = board.grid[dr][dc];
            board.grid[dr][dc]  = piece;
            board.grid[sr][sc]  = '.';

            board.pawnPromotion(dr, dc);

            if (isKingInCheck(whiteTurn)) {
                board.grid[sr][sc]  = piece;
                board.grid[dr][dc]  = captured;
                cout << "Move leaves king in check \n";
                continue;
            }

            whiteTurn = !whiteTurn;
        }
    }
};

// ---------------- MAIN ----------------
int main() {
    Game game;
    game.run();
    return 0;
}