#ifndef GROUPWIDGET_HPP
#define GROUPWIDGET_HPP
#include <QVBoxLayout>
#include <QTextBrowser>
#include <string>
#include <tamandua/types.hpp>
#include <tamandua/message.hpp>

class GroupWidget :
        public QWidget
{
private:
    tamandua::id_number_t gid;
    QString name;
    QVBoxLayout* layout;
    QTextBrowser* browser;
public:
    GroupWidget(QWidget *, tamandua::id_number_t, QString);
    tamandua::id_number_t getGid();
    QString getName();
    QTextBrowser* getTextBrowser();
};

#endif // GROUPWIDGET_HPP
