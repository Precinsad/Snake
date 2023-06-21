#ifndef SNAKEGAME_H
#define SNAKEGAME_H

#include <QObject>
#include <QWidget>
#include <QKeyEvent>
#include <QTimer>

#ifdef UNIT_TESTS
#define private public
#endif

/**
 * @brief Перечисление уровней сложности игры.
 */
enum DifficultyLevel
{
    Easy,   /**< Легкий уровень сложности. */
    Medium, /**< Средний уровень сложности. */
    Hard    /**< Тяжелый уровень сложности. */
};

/**
 * @brief Класс игры "Змейка".
 */
class SnakeGame : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор класса SnakeGame.
     * @param parent Родительский виджет.
     */
    explicit SnakeGame(QWidget *parent = nullptr);

    /**
     * @brief Получить наивысший счет игры.
     * @return Наивысший счет игры.
     */
    int getHighestScore() { return highestScore; };

    /**
     * @brief Запустить игру.
     * @param difficulty Уровень сложности.
     * @param playerName Имя игрока.
     * @param snakeColor Цвет змейки.
     */
    void run(DifficultyLevel difficulty, QString playerName, QColor snakeColor);

    /**
     * @brief Деструктор класса SnakeGame.
     */
    ~SnakeGame();

protected:
    /**
     * @brief Обработчик события перерисовки виджета.
     * @param event Событие перерисовки.
     */
    void paintEvent(QPaintEvent *event);

    /**
     * @brief Обработчик события нажатия клавиши.
     * @param event Событие нажатия клавиши.
     */
    void keyPressEvent(QKeyEvent *event);

private:
    /**
     * @brief Перечисление направлений движения змейки.
     */
    enum Direction
    {
        Up,   /**< Вверх. */
        Down, /**< Вниз. */
        Left, /**< Влево. */
        Right /**< Вправо. */
    };

    /**
     * @brief Структура, представляющая точку на игровом поле.
     */
    struct Point
    {
        int x; /**< Координата X. */
        int y; /**< Координата Y. */
    };

    QVector<Point> snake;       /**< Вектор сегментов змейки. */
    QVector<Point> walls;       /**< Вектор стен. */
    Point fruit;                /**< Позиция фрукта. */
    Direction currentDirection; /**< Текущее направление движения змейки. */
    QTimer *timer;              /**< Таймер для обновления игры. */

    bool testing = false;              /**< Флаг тестирования. */
    int boardSize;                     /**< Размер игрового поля. */
    int tileSize;                      /**< Размер клетки. */
    int score;                         /**< Текущий счет игры. */
    int highestScore;                  /**< Наивысший счет игры. */
    double combo;                      /**< Комбо-множитель. */
    bool isCrossingEnabled;            /**< Разрешение на пересечение змейки. */
    DifficultyLevel currentDifficulty; /**< Текущий уровень сложности. */
    QString playerName;                /**< Имя игрока. */
    QColor snakeColor;                 /**< Цвет змейки. */

    /**
     * @brief Инициализация игры.
     * @param difficultyLevel Уровень сложности игры.
     */
    void initializeGame(DifficultyLevel difficultyLevel);

    /**
     * @brief Обернуть змейку на игровом поле.
     * @param point Точка змейки.
     */
    void wrapSnake(Point &point);

    /**
     * @brief Сгенерировать фрукт на игровом поле.
     */
    void generateFruit();

    /**
     * @brief Проверить столкновение змейки.
     * @return true, если произошло столкновение; в противном случае - false.
     */
    bool checkCollision();

    /**
     * @brief Увеличить змейку.
     */
    void incrementSnake();

    /**
     * @brief Проверить условия победы.
     * @return true, если игрок выиграл; в противном случае - false.
     */
    bool checkWin();

    /**
     * @brief Переместить змейку.
     */
    void moveSnake();

    /**
     * @brief Обработчик события завершения игры.
     */
    void gameOver();

    /**
     * @brief Получить путь для головы змейки.
     * @return Путь для головы змейки.
     */
    QPainterPath getSnakeHeadPath();

private slots:
    /**
     * @brief Обновить состояние игры.
     */
    void updateGame();

signals:
    /**
     * @brief Сигнал изменения счета игры.
     * @param score Новый счет игры.
     */
    void scoreChanged(int score);

    /**
     * @brief Сигнал завершения игры.
     * @param score Счет игры.
     */
    void gameOverEvent(int score);

    /**
     * @brief Сигнал изменения комбо-множителя.
     * @param combo Новый комбо-множитель.
     */
    void comboChanged(int combo);
};

#endif // SNAKEGAME_H
