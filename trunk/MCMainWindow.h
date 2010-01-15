#ifndef MCMAINWINDOW_H
#define MCMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MCMainWindow;
}

class MCMainWindow : public QMainWindow {
    Q_OBJECT
public:
    MCMainWindow(QWidget *parent = 0);
    ~MCMainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MCMainWindow *ui;
};

#endif // MCMAINWINDOW_H
