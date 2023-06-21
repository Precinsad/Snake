#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include "snakegame.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Game;
}
QT_END_NAMESPACE

/**
 * @brief Класс Game
 *
 * Класс Game представляет основное окно игры.
 *
 */
class Game : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор класса Game.
     *
     * @param parent Родительский виджет (по умолчанию nullptr).
     */
    Game(QWidget *parent = nullptr);

    /**
     * @brief Деструктор класса Game.
     *
     */
    ~Game();

    /**
     * @brief Обработка изменения счета.
     *
     * @param score Новое значение счета.
     */
    void handleScoreChanged(int score);

    /**
     * @brief Обработка завершения игры.
     *
     * @param score Счет игры.
     */
    void handleGameOver(int score);

    /**
     * @brief Обработка изменения комбо.
     *
     * @param combo Новое значение комбо.
     */
    void handleComboChanged(int combo);

    /**
     * @brief Подготовка игры.
     *
     * @param playerName Имя игрока.
     * @param snakeColor Цвет змеи.
     * @param difficulty Уровень сложности.
     */
    void prepareGame(QString playerName, QColor snakeColor, DifficultyLevel difficulty);

private:
    Ui::Game *ui;
};

#endif // GAME_H
