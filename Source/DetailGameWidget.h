#ifndef DETAILGAMEWIDGET_H
#define DETAILGAMEWIDGET_H

#include <QWidget>
#include <Database.h>

#include <QWidget>

/** Grid Game Widget class.
 * Widget for each game on the grid game list
*/
class DetailGameWidget : public QWidget
{
    Q_OBJECT

public:
    DetailGameWidget(Game game, QSettings* palette, QString background, QWidget* parent = 0);

private:
    QHBoxLayout* mainLayout;
    QLabel* imageLabel;
    QWidget* gameTitleWidget;
    QWidget* gameDetailsWidget;
    QWidget* creatorInfoWidget;
    void enterEvent(QEvent*) override;
    void leaveEvent(QEvent*) override;
    void paintEvent(QPaintEvent *) override;
    QSettings* palette;
    void styleLabel(QLabel* label);
    bool currentlyHoveredOver;

public slots:

signals:

};

#endif // DETAILGAMEWIDGET_H
