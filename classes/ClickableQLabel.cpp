#include "ClickableQLabel.h"

ClickableQLabel::ClickableQLabel(QWidget *parent) : QLabel(parent) {}

void ClickableQLabel::mousePressEvent(QMouseEvent *ev) {
    emit Mouse_Pressed();
}
