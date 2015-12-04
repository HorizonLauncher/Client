#ifndef _GRIDGAMEWIDGET_H_
#define _GRIDGAMEWIDGET_H_

#include <QWidget>

/** Grid Game Widget class.
 * Widget for each game on the grid game list
*/
class GridGameWidget : public QWidget
{
    Q_OBJECT

public:
    GridGameWidget(QString gameName, int hours, QWidget* parent = 0);

    void mousePressEvent(QMouseEvent* event);

private:
    void init();

public slots:
    void showContextMenu(const QPoint& pos);

signals:
    void clicked();
    void leftClick();
    void rightClick();

    void changeLaunchOpts();
    void removeGame();
};

#endif
