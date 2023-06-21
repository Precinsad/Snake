
#include "game.h"
#include "welcome.h"
#include "snakegame.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    // Создание экземпляра класса QApplication и передача аргументов командной строки
    QApplication a(argc, argv);

    // Создание диалогового окна
    Welcome d;

    // Если диалоговое окно было закрыто с результатом QDialog::Accepted
    if (d.exec() == QDialog::Accepted)
    {
        // Создание экземпляра класса MainWindow
        Game w;

        // Получение имени первого игрока из диалогового окна
        QString pn = d.getPlayerName();

        // Получение имени второго игрока из диалогового окна
        QColor sc = d.getSnakeColor();

        // Получение размера поля из диалогового окна
        int df = d.getDifficulty();

        DifficultyLevel difficulty = (DifficultyLevel)df;

        // Установка параметров MainWindow
        w.prepareGame(pn, sc, difficulty);

        // Отображение главного окна
        w.show();

        // Запуск главного цикла обработки событий QApplication
        return a.exec();
    }
    else
    {
        // Если диалоговое окно было закрыто с результатом QDialog::Rejected, выход из программы
        return 0;
    }
}
