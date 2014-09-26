#ifndef MESSAGEEDIT_HPP
#define MESSAGEEDIT_HPP
#include <QTextEdit>
#include <QKeyEvent>
#include <functional>

class MessageEdit : public QTextEdit
{
    Q_OBJECT
public:
    void setEnterProcess(bool);
    void setEnterCallback(std::function<void()>);
    void keyPressEvent(QKeyEvent *);
private:
    std::function<void()> enter_callback;
    bool enter_process;
};

#endif // MESSAGEEDIT_HPP
