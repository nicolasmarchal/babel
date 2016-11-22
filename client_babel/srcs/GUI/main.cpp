#include <QApplication>
#include <QMainWindow>
#include <QDesktopWidget>
#include <QGridLayout>
#include <QStackedWidget>
#include <QVBoxLayout>
#include  "form.h"
#include "Client.hh"
#include "QTGUI.hh"
#include "init.h"


int main(int argc, char *argv[])
{
  //    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    IEventManager *app = new QTEventManager(argc, argv);
    Client client;

//    try
//    {
//        client.connect(std::string("10.20.86.38"), atoi("4242"));
//    } catch (NetworkError *e)
//    {
//        std::cout << e->what() << std::endl;
//        delete e;
//        return (1);
//    }
    std::cout << "Connected" << std::endl;
    client.loop(*app);
    delete app;
    return (0);
}
