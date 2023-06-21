
#include "welcome.h"
#include "./ui_welcome.h"

Welcome::Welcome(QWidget *parent)
    : QDialog(parent), ui(new Ui::Welcome)
{
    ui->setupUi(this);
}

QString Welcome::getPlayerName()
{
    QString playerName = ui->playerName->text();

    if (playerName.isEmpty())
    {
        playerName = "Player";
    }
    return playerName;
}

int Welcome::getDifficulty()
{
    return ui->difficulty->currentIndex();
}

QColor Welcome::getSnakeColor()
{
    // Check if the hex color code is valid
    QColor color(ui->snakeColor->text());
    if (color.isValid())
    {
        return color;
    }

    // If not valid, return a random color from a predefined list
    std::vector<QColor> randomColors = {
        QColor("#00FF00"), // Green
        QColor("#0000FF"), // Blue
        QColor("#FFFF00"), // Yellow
        QColor("#FF00FF"), // Magenta
        QColor("#00FFFF")  // Cyan
    };

    // Generate a random index
    int index = rand() % randomColors.size();

    return randomColors[index];
}

Welcome::~Welcome()
{
    delete ui;
}
