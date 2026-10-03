#include "chessboardwidget.h"
#include <QDebug>

ChessBoardWidget::ChessBoardWidget(QWidget *parent)
    : QWidget(parent), game(nullptr), selectedRow(-1), selectedCol(-1), squareSize(62)
{
    setMinimumSize(500, 500);
    lastFromRow = lastFromCol = lastToRow = lastToCol = -1;
}

void ChessBoardWidget::setGame(plateau *game)
{
    this->game = game;
    update();
}

void ChessBoardWidget::updateBoard()
{
    update();
}

void ChessBoardWidget::paintEvent(QPaintEvent *event)
{
    if (!game) return;

    QPainter painter(this);
    squareSize = qMin(width(), height()) / 8;

    drawBoard(painter);
    drawPieces(painter);
}

void ChessBoardWidget::drawBoard(QPainter &painter)
{
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            QRect rect(col * squareSize, row * squareSize, squareSize, squareSize);

            // Alternate colors
            if ((row + col) % 2 == 0)
                painter.fillRect(rect, QColor(240, 217, 181));
            else
                painter.fillRect(rect, QColor(181, 136, 99));

            // Highlight selected square
            if (selectedRow == row && selectedCol == col)
                painter.fillRect(rect, QColor(0, 255, 0, 100));

            // Highlight last move
            if ((lastFromRow == row && lastFromCol == col) ||
                (lastToRow == row && lastToCol == col))
                painter.fillRect(rect, QColor(255, 255, 0, 100));

            // Draw border
            painter.setPen(QPen(Qt::black, 1));
            painter.drawRect(rect);
        }
    }
}

void ChessBoardWidget::drawPieces(QPainter &painter)
{
    painter.setFont(QFont("Arial", squareSize * 0.6, QFont::Bold));

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            // Access the board using the plat array directly
            int piece = game->plat[row][col];
            if (piece != 0) {
                QRect rect(col * squareSize, row * squareSize, squareSize, squareSize);
                painter.setPen(getPieceColor(piece));
                painter.drawText(rect, Qt::AlignCenter, getPieceSymbol(piece));
            }
        }
    }
}

QString ChessBoardWidget::getPieceSymbol(int piece)
{
    int absPiece = abs(piece);
    switch(absPiece) {
    case 1: return "♙";  // Pawn
    case 2: return "♖";  // Rook
    case 3: return "♘";  // Knight
    case 4: return "♗";  // Bishop
    case 5: return "♕";  // Queen
    case 6: return "♔";  // King
    default: return "?";
    }
}

QColor ChessBoardWidget::getPieceColor(int piece)
{
    return (piece > 0) ? Qt::white : Qt::black;
}

void ChessBoardWidget::mousePressEvent(QMouseEvent *event)
{
    if (!game) return;

    // Use position() instead of x() and y() to avoid deprecation warnings
    int col = event->position().x() / squareSize;
    int row = event->position().y() / squareSize;

    if (row < 0 || row > 7 || col < 0 || col > 7) return;

    if (selectedRow == -1) {
        int currentPlayer = (game->get_tour() % 2 != 0) ? 1 : 0;
        if (game->couleur_piece(row, col) == currentPlayer) {
            selectedRow = row;
            selectedCol = col;
            update();
        }
    } else {
        emit moveMade(selectedRow, selectedCol, row, col);
        lastFromRow = selectedRow;
        lastFromCol = selectedCol;
        lastToRow = row;
        lastToCol = col;
        selectedRow = -1;
        selectedCol = -1;
        update();
    }
}