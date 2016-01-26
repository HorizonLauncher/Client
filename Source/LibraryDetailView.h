#ifndef LIBRARYDETAILVIEW_H
#define LIBRARYDETAILVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QSettings>
class LibraryDetailView : public QWidget
{
    Q_OBJECT
public:
    LibraryDetailView(QSettings* p, QWidget *parent = 0);

signals:

public slots:

private:
    QVBoxLayout* mainLayout;
    void refreshGames();
    QSettings* palette;
};

#endif // LIBRARYDETAILVIEW_H
