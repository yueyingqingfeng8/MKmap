#include "wgaddkeys.h"
#include "ui_wgaddkeys.h"

WgAddKeys::WgAddKeys(int &index, QWidget *parent) :
    WgKeysListItem(index, parent),
    ui(new Ui::WgAddKeys)
{
    ui->setupUi(this);
}

WgAddKeys::~WgAddKeys()
{
    delete ui;
}

void WgAddKeys::on_pushButton_clicked()
{
    emit signalAddkeysWidget(m_index);
}

