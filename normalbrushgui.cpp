#include "normalbrushgui.h"
#include "ui_normalbrushgui.h"

NormalBrushGui::NormalBrushGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NormalBrushGui)
{
  ui->setupUi(this);
  connect(ui->sliderRadius,SIGNAL(valueChanged(int)),this, SIGNAL(radius_changed(int)));
}

NormalBrushGui::~NormalBrushGui()
{
  delete ui;
}

QColor NormalBrushGui::get_normal(){
  return ui->normalSelectorWidget->get_normal();
}
