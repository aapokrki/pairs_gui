/* Pairs_gui
 *
 * Program author:
 * Name: Aapo Kärki
 * Student number: H292001
 * UserID: gxaaka
 * E-Mail: aapo.karki@tuni.fi
 * Course: COMP.CS.110 K2021
 *
 * Read instructions.txt
 * */

#include "mainwindow.hh"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
