#include "WindowManager.h"

#include <QLabel>

WindowManager::WindowManager(QString title, QString contents) {
    QWidget *window = new QWidget(nullptr, Qt::Dialog);
    window->resize(300, 500);
    window->setFixedSize(window->size());
    window->setWindowFlags(window->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    window->setWindowTitle("AllCash :: " + title);

    QLabel *text = new QLabel(window);
    text->setText(contents.replace("\\n", "<br>"));
    text->setFont(QFont("Trebuchet MS", 10));
    text->move(10, 10);
    text->setFixedWidth(window->width() - 20);
    text->setWordWrap(true);

    window->setFixedHeight(text->sizeHint().height() + 20);
    window->show();
}
