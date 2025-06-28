#include "quizapp.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QuizApp w;
    w.show();
    return a.exec();
}
