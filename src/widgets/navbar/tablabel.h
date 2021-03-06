#ifndef TABLABEL_H
#define TABLABEL_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QSettings>

class TabLabel : public QLabel
{
public:
    TabLabel(QWidget*, QSettings*);
    bool isActive;

protected:
    void paintEvent(QPaintEvent*);

QSettings* palette;
};

#endif // TABLABEL_H
