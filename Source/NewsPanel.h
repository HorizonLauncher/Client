#ifndef NEWSPANEL_H
#define NEWSPANEL_H

#include <QWidget>

namespace Ui {
class NewsPanel;
}

class NewsPanel : public QWidget
{
    Q_OBJECT

public:
    explicit NewsPanel(QWidget *parent = 0);
    ~NewsPanel();

private:
    Ui::NewsPanel *ui;
};

#endif // NEWSPANEL_H
