#ifndef NEWSFEEDCHOOSERWINDOW_H
#define NEWSFEEDCHOOSERWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QSettings>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QLineEdit>

class NewsFeedChooserWindow : public QDialog
{
    Q_OBJECT
public:
    explicit NewsFeedChooserWindow(QWidget *parent = 0);

signals:

public slots:
    void onAddURLButtonClicked();

private:
  QString newsSettingsFilePath;
  void loadURLsFromSettings();
  void saveURLs();
  void createURLLabels();
  QList<QString> urls;
  QList<QString> newUrls;
  QWidget* labels;
  QVBoxLayout* labelLayout;
  QVBoxLayout* layout;
  void closeEvent(QCloseEvent* event) override;

};

#endif // NEWSFEEDCHOOSERWINDOW_H
