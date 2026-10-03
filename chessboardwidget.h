#ifndef CHESSBOARDWIDGET_H
#define CHESSBOARDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QVector>
#include <QPoint>
#include "plateau.h"

class ChessBoardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChessBoardWidget(QWidget *parent = nullptr);
    void setGame(plateau *game);
    void updateBoard();

signals:
    void moveMade(int fromRow, int fromCol, int toRow, int toCol);
    void squareClicked(int row, int col);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    void drawBoard(QPainter &painter);
    void drawPieces(QPainter &painter);
    QString getPieceSymbol(int piece);
    QColor getPieceColor(int piece);

    plateau *game;
    int selectedRow;
    int selectedCol;
    int squareSize;
    int lastFromRow, lastFromCol, lastToRow, lastToCol;
};

#endif