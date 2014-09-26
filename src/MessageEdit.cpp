#include "MessageEdit.hpp"

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
        enter_callback();
    } else
    {
        QTextEdit::keyPressEvent(event);
    }
}
