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
    void loadSettings();
    void saveSettings();
    void changeEvent(QEvent *e);

private slots:
    void on_actionAdd_Project_File_triggered();
    void on_actionRemove_Share_triggered();
    void on_actionAdd_Project_Folder_triggered();
    void on_actionPreferences_triggered();

private:
    Ui::MCMainWindow *ui;

    //LocalRepositoryModel    m_localModel;
};

#endif // MCMAINWINDOW_H
