#include "dishturnpagebar.h"
#include "ui_dishturnpagebar.h"
#include "Tools/tool_00_environment.h"
#include <QSqlQuery>
#include <QDebug>

DishTurnPageBar::DishTurnPageBar(QString *controlwin_username, QScrollArea* scrollArea, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DishTurnPageBar),
    controlwin_username(controlwin_username),
    scrollArea(scrollArea),
    scrollAreaContents(parent)
{
    ui->setupUi(this);
}

DishTurnPageBar::~DishTurnPageBar()
{
    delete ui;
}

bool DishTurnPageBar::addDish(DishShowBar *t)
{
    return dishManager.addDish(t);
}

bool DishTurnPageBar::deleteDish(QString dishName)
{
    return dishManager.deleteDish(dishName);
}

DishShowBar *DishTurnPageBar::findDish(QString dishName)
{
    return dishManager.findDish(dishName);
}

int DishTurnPageBar::sizeDish()
{
    return dishManager.sizeDish();
}

void DishTurnPageBar::dishInitfromDB()
{
    QSqlQuery query(*DB);
    QString sql = QString("select * from dishesInfo order by DishIndex asc;");
    query.exec(sql);

    while(query.next())
    {
        dishManager.addDish(new DishShowBar(controlwin_username,query.value("DishImagePath").toString(),
                                            query.value("DishName").toString(),
                                            query.value("DishMoney").toString(),
                                            query.value("DishNum").toString(),
                                            query.value("DishIndex").toString(),this->scrollAreaContents));
    }

    dishManager.addDish(new DishShowBar(controlwin_username,"","请添加菜品",
                                        "请添加价格","请添加数量","-1",this->scrollAreaContents));
}

void DishTurnPageBar::dishShow()
{
    for(int i = 0;i < dishManager.sizeDish();i++) dishManager[i]->hide();
//    qDebug() << adminFlag;
    int validDishSize = *controlwin_username == "admin" ?
                dishManager.sizeDish() : dishManager.sizeDish() - 1;

    int i = curIndex * pageShowNum;
    int margin = (curIndex + 1) * pageShowNum;
    int valid_margin = (validDishSize < margin ? validDishSize : margin);

    for(;i < valid_margin;i++)
    {
        dishManager[i]->move(x + ((i % pageShowNum) % 3) * 220, y + (((i % pageShowNum)) / 3) * 290);
        dishManager[i]->show();
    }

    this->move(170,y + ((((--i % pageShowNum)) / 3) + 1) * 290 + 20);
    scrollArea->verticalScrollBar()->setValue(0);

}

void DishTurnPageBar::on_curIndex_lineEdit_textEdited(const QString &arg1)
{
    // 预判
    int t_index = arg1.toInt();
    if(t_index < 1 || t_index > (dishManager.sizeDish() + pageShowNum - 1) / pageShowNum)
    {
        ui->pageIndex_lineEdit->setText(QString::number(curIndex + 1));
        return;
    }

    curIndex = t_index - 1;
    // 展示
    dishShow();
}

void DishTurnPageBar::on_left_ptn_clicked()
{
    // 预判
    curIndex = curIndex > 0 ? curIndex - 1 : 0;

    // 展示
    dishShow();

    ui->pageIndex_lineEdit->setText(QString::number(curIndex + 1));
}

void DishTurnPageBar::on_right_ptn_clicked()
{
    // 预判
    curIndex = curIndex <  (dishManager.sizeDish() + pageShowNum - 1) / pageShowNum - 1 ?
                curIndex + 1 : (dishManager.sizeDish() + pageShowNum - 1) / pageShowNum - 1;

    // 展示
    dishShow();

    ui->pageIndex_lineEdit->setText(QString::number(curIndex + 1));

}

void DishTurnPageBar::on_pageIndex_lineEdit_textChanged(const QString &arg1)
{
    // 预判
    int t_index = arg1.toInt();
    if(t_index < 1 || t_index > (dishManager.sizeDish() + pageShowNum - 1) / pageShowNum)
    {
        ui->pageIndex_lineEdit->setText(QString::number(curIndex + 1));
        return;
    }

    curIndex = t_index - 1;
    // 展示
    dishShow();
}
