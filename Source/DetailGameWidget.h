#ifndef DETAILGAMEWIDGET_H
#define DETAILGAMEWIDGET_H

#include <QWidget>
#include <Database.h>
class DetailGameWidget : public QWidget
{
public:
    DetailGameWidget(Game game, QWidget* parent = 0);
private:
    QHBoxLayout* mainLayout;
    QLabel* imageLabel;
    QWidget* gameTitleWidget;
    QWidget* gameDetailsWidget;
};

#endif // DETAILGAMEWIDGET_H
