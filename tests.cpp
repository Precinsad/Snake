#include <QTest>
#include <QSignalSpy>

#define UNIT_TESTS
#include "snakegame.h"

class SnakeGameTest : public QObject
{
    Q_OBJECT

private slots:
    void testInitializeGame();
    void testGenerateFruit();
    void testCheckCollision();
    void testMoveSnake();
    void testGameOver();
};

void SnakeGameTest::testInitializeGame()
{
    SnakeGame snakeGame;

    QCOMPARE(snakeGame.getHighestScore(), 0);
    QCOMPARE(snakeGame.currentDifficulty, Easy);
    QCOMPARE(snakeGame.playerName, QString("Player"));
    QCOMPARE(snakeGame.snakeColor, QColor(Qt::green));

    snakeGame.initializeGame(Medium);

    QCOMPARE(snakeGame.currentDifficulty, Medium);
    QCOMPARE(snakeGame.isCrossingEnabled, true);
    QCOMPARE(snakeGame.walls.size(), 3);
}

void SnakeGameTest::testGenerateFruit()
{
    SnakeGame snakeGame;

    snakeGame.initializeGame(Easy);

    snakeGame.generateFruit();

    QVERIFY(snakeGame.fruit.x >= 0 && snakeGame.fruit.x < snakeGame.boardSize);
    QVERIFY(snakeGame.fruit.y >= 0 && snakeGame.fruit.y < snakeGame.boardSize);
}

void SnakeGameTest::testCheckCollision()
{
    SnakeGame snakeGame;

    snakeGame.initializeGame(Easy);

    snakeGame.snake = {SnakeGame::Point{10, 8}, SnakeGame::Point{10, 9}};
    snakeGame.walls = {SnakeGame::Point{10, 8}};

    QVERIFY(snakeGame.checkCollision());

    snakeGame.snake = {SnakeGame::Point{10, 10}, SnakeGame::Point{10, 9}};
    snakeGame.fruit = SnakeGame::Point{9, 8};

    QVERIFY(!snakeGame.checkCollision());
}

void SnakeGameTest::testMoveSnake()
{

    SnakeGame snakeGame;

    snakeGame.initializeGame(Easy);

    snakeGame.snake = {SnakeGame::Point{0, 0}};

    snakeGame.currentDirection = SnakeGame::Up;

    snakeGame.moveSnake();

    QCOMPARE(snakeGame.snake.first().x, 0);
    QCOMPARE(snakeGame.snake.first().y, 19);
}

void SnakeGameTest::testGameOver()
{
    SnakeGame snakeGame;
    snakeGame.testing = true;

    snakeGame.initializeGame(Easy);

    snakeGame.snake = {SnakeGame::Point{10, 10}, SnakeGame::Point{10, 9}};
    snakeGame.fruit = SnakeGame::Point{10, 10};

    QSignalSpy scoreChangedSpy(&snakeGame, SIGNAL(scoreChanged(int)));
    QSignalSpy gameOverSpy(&snakeGame, SIGNAL(gameOverEvent(int)));

    QVERIFY(!snakeGame.checkCollision());

    snakeGame.gameOver();

    QCOMPARE(scoreChangedSpy.count(), 1);
    QCOMPARE(gameOverSpy.count(), 1);
}

QTEST_MAIN(SnakeGameTest)
#include "tests.moc"
