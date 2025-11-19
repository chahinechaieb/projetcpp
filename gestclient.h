#ifndef GESTCLIENT_H
#define GESTCLIENT_H

#include <QMainWindow>
#include "client.h"

namespace Ui {
class gestclient;
}

class gestclient : public QMainWindow
{
    Q_OBJECT

public:
    explicit gestclient(QWidget *parent = nullptr);
    ~gestclient();

private slots:
    void on_btnajouter_clicked();
    void on_btnsupprimer_clicked();
    void on_btnmodifier_clicked();

private:
    Ui::gestclient *ui;
    client Etmp;
};

#endif // GESTCLIENT_H
