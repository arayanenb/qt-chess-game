#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), game(nullptr), gameMode(0), waitingForAI(false)
{
    // Create central widget and layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Create top bar with controls
    QHBoxLayout *controlLayout = new QHBoxLayout();

    QLabel *modeLabel = new QLabel("Game Mode:", this);
    gameModeCombo = new QComboBox(this);
    gameModeCombo->addItem("Player vs Player");
    gameModeCombo->addItem("Player vs AI");

    resetButton = new QPushButton("Reset Game", this);
    statusLabel = new QLabel("White's turn", this);
    statusLabel->setStyleSheet("font-size: 14px; font-weight: bold; padding: 5px;");

    controlLayout->addWidget(modeLabel);
    controlLayout->addWidget(gameModeCombo);
    controlLayout->addWidget(resetButton);
    controlLayout->addStretch();
    controlLayout->addWidget(statusLabel);

    // Create chess board
    board = new ChessBoardWidget(this);

    // Add to main layout
    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(board);

    setCentralWidget(centralWidget);

    // Initialize game
    game = new plateau();
    board->setGame(game);

    // Connect signals
    connect(gameModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onGameModeChanged);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetGame);
    connect(board, &ChessBoardWidget::moveMade, this, &MainWindow::onMoveMade);

    setWindowTitle("Chess Game");
    resize(600, 650);

    updateStatus();
}

MainWindow::~MainWindow()
{
    delete game;
}

void MainWindow::onGameModeChanged(int index)
{
    gameMode = index;
    onResetGame();
}

void MainWindow::onMoveMade(int fromRow, int fromCol, int toRow, int toCol)
{
    if (waitingForAI) return;

    // In AI mode, only allow moves when it's player's turn (white = turn 1,3,5...)
    if (gameMode == 1) {
        int currentPlayer = (game->get_tour() % 2 != 0) ? 1 : 0;
        if (currentPlayer != 1) { // Not player's turn (player is white)
            statusLabel->setText("AI is thinking...");
            return;
        }
    }

    makeMove(fromRow, fromCol, toRow, toCol);
}

void MainWindow::makeMove(int fromRow, int fromCol, int toRow, int toCol)
{
    if (game->eval(fromRow, fromCol, toRow, toCol)) {
        // AJOUTER ICI - Vérifier que le mouvement ne met pas son propre roi en échec
        int currentPlayer = (game->get_tour() % 2 != 0) ? 1 : 0;

        // Simuler le mouvement
        int pieceCapturee = game->plat[toRow][toCol];
        int pieceDeplacee = game->plat[fromRow][fromCol];
        game->plat[toRow][toCol] = pieceDeplacee;
        game->plat[fromRow][fromCol] = 0;

        bool enEchec = game->estEnEchec(currentPlayer);

        // Annuler le mouvement
        game->plat[fromRow][fromCol] = pieceDeplacee;
        game->plat[toRow][toCol] = pieceCapturee;

        if (enEchec) {
            statusLabel->setText("Vous ne pouvez pas mettre votre roi en échec !");
            QTimer::singleShot(1000, this, [this]() { updateStatus(); });
            return;
        }

        // Exécuter le mouvement
        game->deplacerPiece(fromRow, fromCol, toRow, toCol);
        board->updateBoard();
        updateStatus();

        if (gameMode == 1 && !waitingForAI) {
            int currentPlayerAfter = (game->get_tour() % 2 != 0) ? 1 : 0;
            if (currentPlayerAfter == 0) {
                QTimer::singleShot(500, this, &MainWindow::onAITurn);
                waitingForAI = true;
                statusLabel->setText("AI is thinking...");
            }
        }
    } else {
        statusLabel->setText("Invalid move! Try again.");
        QTimer::singleShot(1000, this, [this]() { updateStatus(); });
    }
}

void MainWindow::onAITurn()
{
    if (gameMode == 1 && waitingForAI) {
        // Check if it's really AI's turn
        int currentPlayer = (game->get_tour() % 2 != 0) ? 1 : 0;
        if (currentPlayer == 0) {
            game->deplacerIA();
            board->updateBoard();
            updateStatus();
        }

        waitingForAI = false;
    }
}

void MainWindow::onResetGame()
{
    delete game;
    game = new plateau();
    board->setGame(game);
    waitingForAI = false;
    updateStatus();
    board->updateBoard();
}

void MainWindow::updateStatus()
{
    QString status;
    int currentPlayer = (game->get_tour() % 2 != 0) ? 1 : 0;

    // AJOUTER CES VÉRIFICATIONS
    if (game->estEchecEtMat(currentPlayer)) {
        QString gagnant = (currentPlayer == 1) ? "Noir" : "Blanc";
        status = gagnant + " gagne ! (Échec et mat)";
        statusLabel->setText(status);
        statusLabel->setStyleSheet("font-size: 14px; font-weight: bold; padding: 5px; background-color: #ff4444; color: white;");
        return;
    }
    if (game->estPat(currentPlayer)) {
        statusLabel->setText("Pat ! Match nul.");
        statusLabel->setStyleSheet("font-size: 14px; font-weight: bold; padding: 5px; background-color: #ffaa00; color: black;");
        return;
    }

    // Code existant
    if (currentPlayer == 1)
        status = "White's turn";
    else
        status = "Black's turn";

    if (gameMode == 1 && currentPlayer == 0 && !waitingForAI)
        status += " (AI)";

    statusLabel->setText(status);
    statusLabel->setStyleSheet("font-size: 14px; font-weight: bold; padding: 5px; background-color: " +
                               QString(currentPlayer == 1 ? "#f0f0f0" : "#333333") +
                               "; color: " + QString(currentPlayer == 1 ? "black" : "white") + ";");
}