#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include "plateau.h"
#include "chessboardwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onGameModeChanged(int index);
    void onMoveMade(int fromRow, int fromCol, int toRow, int toCol);
    void onResetGame();
    void onAITurn();

private:
    void updateStatus();
    void makeMove(int fromRow, int fromCol, int toRow, int toCol);

    plateau *game;
    ChessBoardWidget *board;
    QComboBox *gameModeCombo;
    QLabel *statusLabel;
    QPushButton *resetButton;
    int gameMode; // 0: Player vs Player, 1: Player vs AI
    bool waitingForAI;
};

#endif