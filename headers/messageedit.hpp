#ifndef MESSAGEEDIT_HPP
#define MESSAGEEDIT_HPP
#include <QTextEdit>
#include <QKeyEvent>
#include <functional>
#include <deque>

class MessageEdit : public QTextEdit
{
    Q_OBJECT
public:
    MessageEdit(QWidget *);
    void setEnterProcess(bool);
    void setEnterCallback(std::function<void()>);
    void keyPressEvent(QKeyEvent *);
private:
    std::function<void()> enter_callback;
    bool enter_process;
    std::deque<QString> last_sent;
    std::deque<QString>::reverse_iterator last_sent_it;

    QPalette old_msg, new_msg;
};

#endif // MESSAGEEDIT_HPP
