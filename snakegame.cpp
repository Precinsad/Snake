#include "snakegame.h"
#include <QPainter>
#include <QDebug>
#include <QMessageBox>

SnakeGame::SnakeGame(QWidget *parent)
    : QWidget(parent), boardSize(20), tileSize(20), score(0), combo(0), highestScore(0), isCrossingEnabled(true), currentDifficulty(Easy),
      playerName("Player"), snakeColor(Qt::green)
{
    setFixedSize(boardSize * tileSize, boardSize * tileSize);
    setFocusPolicy(Qt::StrongFocus);
    initializeGame(currentDifficulty);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGame()));
}

void SnakeGame::run(DifficultyLevel difficulty, QString player, QColor color)
{
    currentDifficulty = difficulty;
    playerName = player;
    snakeColor = color;
    initializeGame(currentDifficulty);

    timer->start(100);
}
SnakeGame::~SnakeGame()
{
    delete timer;
}

void SnakeGame::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.drawRect(0, 0, width() - 1, height() - 1);

    // Draw the walls
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    for (const Point &point : walls)
    {
        painter.drawRect(point.x * tileSize, point.y * tileSize, tileSize, tileSize);
    }

    // Draw the snake
    for (const Point &point : snake)
    {
        painter.fillRect(point.x * tileSize, point.y * tileSize, tileSize, tileSize, snakeColor);
    }

    // Draw the fruit
    painter.fillRect(fruit.x * tileSize, fruit.y * tileSize, tileSize, tileSize, Qt::red);
}

void SnakeGame::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_W:
    case Qt::Key_Up:
        if (currentDirection != Down)
            currentDirection = Up;
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        if (currentDirection != Up)
            currentDirection = Down;
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        if (currentDirection != Right)
            currentDirection = Left;
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        if (currentDirection != Left)
            currentDirection = Right;
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void SnakeGame::initializeGame(DifficultyLevel difficultyLevel)
{
    score = 0;
    combo = 0;
    snake.clear();
    snake.append({10, 17});
    snake.append({10, 18});
    snake.append({10, 19});

    currentDirection = Up;
    currentDifficulty = difficultyLevel;

    walls.clear();

    switch (currentDifficulty)
    {
    case Easy:
        isCrossingEnabled = true;
        break;
    case Medium:
        isCrossingEnabled = true;
        walls.append({3, 3});
        walls.append({3, 4});
        walls.append({3, 5});
        break;
    case Hard:
        isCrossingEnabled = false;
        walls.append({3, 3});
        walls.append({3, 4});
        walls.append({6, 15});
        walls.append({6, 16});
        walls.append({6, 17});
        walls.append({6, 14});
        walls.append({7, 14});
        walls.append({8, 14});
        walls.append({9, 17});
        walls.append({9, 14});
        walls.append({13, 8});
        walls.append({14, 8});
        walls.append({15, 8});
        break;
    }

    generateFruit();
}

void SnakeGame::generateFruit()
{
    QVector<Point> emptySpaces;

    // Find all available empty spaces
    for (int x = 0; x < boardSize; ++x)
    {
        for (int y = 0; y < boardSize; ++y)
        {
            bool isProtected = false;
            for (const Point &point : snake)
            {
                if (point.x == x && point.y == y)
                {
                    isProtected = true;
                    break;
                }
            }
            for (const Point &point : walls)
            {
                if (point.x == x && point.y == y)
                {
                    isProtected = true;
                    break;
                }
            }
            if (!isProtected)
                emptySpaces.append({x, y});
        }
    }

    // If there are no empty spaces left, end the game
    if (emptySpaces.isEmpty())
    {
        gameOver();
        return;
    }

    // Randomly select one of the empty spaces for fruit placement
    int randomIndex = rand() % emptySpaces.size();
    fruit = emptySpaces[randomIndex];
}

void SnakeGame::wrapSnake(Point &point)
{
    if (point.x < 0)
        point.x = boardSize - 1;
    else if (point.x >= boardSize)
        point.x = 0;

    if (point.y < 0)
        point.y = boardSize - 1;
    else if (point.y >= boardSize)
        point.y = 0;
}

bool SnakeGame::checkCollision()
{
    const Point &head = snake.first();

    // Check for collision with walls
    for (const Point &point : walls)
    {
        if (point.x == head.x && point.y == head.y)
            return true;
    }

    // Check for collision with walls
    if (!isCrossingEnabled && (head.x < 0 || head.x >= boardSize || head.y < 0 || head.y >= boardSize))
        return true;

    // Check for collision with the snake's body
    for (int i = 1; i < snake.size(); ++i)
    {
        if (snake.at(i).x == head.x && snake.at(i).y == head.y)
            return true;
    }

    // Check for collision with the fruit
    if (head.x == fruit.x && head.y == fruit.y)
    {
        if (combo < 10)
        {
            score++;
        }
        else
        {
            score += int(combo / 10);
        }
        combo += rand() % 50;
        if (combo > 100)
        {
            combo = 100.0;
        }
        emit scoreChanged(score);
        incrementSnake();
        generateFruit();
        return false;
    }
    else
    {
        if (combo < .5)
        {
            combo = 0;
        }
        else
        {
            combo -= 1;
        }
    }
    emit comboChanged((int)combo);

    return false;
}

void SnakeGame::moveSnake()
{
    Point head = snake.first();

    switch (currentDirection)
    {
    case Up:
        head.y--;
        break;
    case Down:
        head.y++;
        break;
    case Left:
        head.x--;
        break;
    case Right:
        head.x++;
        break;
    }

    if (isCrossingEnabled)
        wrapSnake(head);

    snake.prepend(head);
    snake.removeLast();

    if (checkCollision())
        gameOver();

    update();
}

bool SnakeGame::checkWin()
{
    int gridSize = boardSize - 2; // Account for the walls

    if (snake.size() == gridSize * gridSize)
        return true;

    return false;
}

void SnakeGame::incrementSnake()
{
    Point tail = snake.last();

    switch (currentDirection)
    {
    case Up:
        tail.y++;
        break;
    case Down:
        tail.y--;
        break;
    case Left:
        tail.x++;
        break;
    case Right:
        tail.x--;
        break;
    }

    snake.append(tail);
}

void SnakeGame::gameOver()
{
    timer->stop();
    if (highestScore < score)
    {
        highestScore = score;
    }
    emit gameOverEvent(score);
    if (checkWin() && !testing)
    {
        QMessageBox::information(this, "Игра окончена", "Игрок " + playerName + " победил!");
    }
    else if (!testing)
    {
        QMessageBox::information(this, "Игра окончена", "Игрок " + playerName + " набрал " + QString::number(score) + " очков.");
    }
    initializeGame(currentDifficulty);
    timer->start(100);
}

void SnakeGame::updateGame()
{
    moveSnake();
}
