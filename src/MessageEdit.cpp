#include "MessageEdit.hpp"
#include <QMessageBox>

MessageEdit::MessageEdit(QWidget *parent) :
        QTextEdit(parent)
{
    old_msg.setColor(QPalette::Base, QColor(0xea, 0xff, 0x63));
    new_msg.setColor(QPalette::Base, QColor(0xff, 0xff, 0xff));
    setPalette(new_msg);
}

void MessageEdit::setEnterProcess(bool process)
{
    enter_process = process;
}

void MessageEdit::setEnterCallback(std::function<void ()> callback)
{
    enter_callback = callback;
}

void MessageEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return && !(event->modifiers() & Qt::KeyboardModifier::ShiftModifier) && enter_process)
    {
        last_sent.push_back(toPlainText());
        enter_callback();
    } else if (event->key() == Qt::Key_Escape)
    {
        last_sent_it = last_sent.rend();
        setPalette(new_msg);
        clear();
        update();
    } else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down)
    {

    } else
    {
        QTextEdit::keyPressEvent(event);
    }
}
