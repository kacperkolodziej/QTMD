#include "groupwidget.hpp"

GroupWidget::GroupWidget(QWidget *parent, tamandua::id_number_t gid_, QString name_) :
    QWidget(parent),
    gid(gid_),
    name(name_),
    layout(new QVBoxLayout(this)),
    browser(new QTextBrowser(this))
{
    layout->addWidget(browser);
    browser->setOpenLinks(true);
    browser->setOpenExternalLinks(true);
}

tamandua::id_number_t GroupWidget::getGid()
{
    return gid;
}

QString GroupWidget::getName()
{
    return name;
}

QTextBrowser* GroupWidget::getTextBrowser()
{
    return browser;
}
