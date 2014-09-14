#include <QApplication>
#include <iostream>

int main(int argc, char** argv) {
    QApplication a(argc, argv);
    std::cout << "hello DRILL";
    return a.exec();
}
