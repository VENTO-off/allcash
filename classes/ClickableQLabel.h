#ifndef CLICKABLEQLABEL_H
#define CLICKABLEQLABEL_H

#include <QLabel>
#include <QMouseEvent>

class ClickableQLabel : public QLabel {
    Q_OBJECT
public:
    explicit ClickableQLabel(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *ev);

signals:
    void Mouse_Pressed();

public slots:

};

#endif
