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

    void filterGames(QString searchString);

private:
    QVBoxLayout* mainLayout;
    void refreshGames();
    QSettings* palette;
    QList<QWidget*> gamesWidgets;

    QString searchString;
};

#endif // LIBRARYDETAILVIEW_H
