#ifndef LIBRARYDETAILVIEW_H
#define LIBRARYDETAILVIEW_H

#include <QWidget>
#include <QVBoxLayout>

class LibraryDetailView : public QWidget
{
    Q_OBJECT
public:
    LibraryDetailView(QWidget *parent = 0);

signals:

public slots:

private:
    QVBoxLayout* mainLayout;
    void refreshGames();
};

#endif // LIBRARYDETAILVIEW_H
