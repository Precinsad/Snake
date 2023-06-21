
#include "game.h"
#include "./ui_game.h"

Game::Game(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Game)
{
    ui->setupUi(this);
    connect(ui->snake, &SnakeGame::scoreChanged, this, &Game::handleScoreChanged);
    connect(ui->snake, &SnakeGame::gameOverEvent, this, &Game::handleGameOver);
    connect(ui->snake, &SnakeGame::comboChanged, this, &Game::handleComboChanged);
}

void Game::handleScoreChanged(int score)
{
    ui->score->setText("Счет: " + QString::number(score));
}

void Game::handleGameOver(int score)
{
    ui->score->setText("Счет: 0");
    ui->high_score->setText("Рекорд: " + QString::number(ui->snake->getHighestScore()));
}

void Game::handleComboChanged(int combo)
{
    ui->combo->setValue(combo);
}

void Game::prepareGame(QString playerName, QColor snakeColor, DifficultyLevel difficulty)
{
    ui->snake->run(difficulty, playerName, snakeColor);
}

Game::~Game()
{
    delete ui;
}
