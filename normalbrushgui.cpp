#include "normalbrushgui.h"
#include "ui_normalbrushgui.h"

NormalBrushGui::NormalBrushGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NormalBrushGui)
{
  ui->setupUi(this);
  connect(ui->sliderRadius,SIGNAL(valueChanged(int)),this, SIGNAL(radius_changed(int)));
  connect(ui->sliderMix,SIGNAL(valueChanged(int)),this, SIGNAL(mix_changed(int)));
  connect(ui->sliderHardness,SIGNAL(valueChanged(int)),this,SIGNAL(hardness_changed(int)));
}

NormalBrushGui::~NormalBrushGui()
{
  delete ui;
}

QColor NormalBrushGui::get_normal(){
  return ui->normalSelectorWidget->get_normal();
}

void NormalBrushGui::on_buttonBrush_toggled(bool checked)
{
  if (checked){
    brushSelected_changed(true);
    selected_changed(true);
    eraserSelected_changed(false);
    ui->buttonEraser->setChecked(false);
  }else{
    if (!ui->buttonEraser->isChecked()){
      selected_changed(false);
    }
  }
}

void NormalBrushGui::on_buttonEraser_toggled(bool checked)
{
  if (checked){
    eraserSelected_changed(true);
    selected_changed(true);
    brushSelected_changed(false);
    ui->buttonBrush->setChecked(false);
  }else{
    if (!ui->buttonBrush->isChecked()){
      selected_changed(false);
    }
  }
}

void NormalBrushGui::on_radioLines_toggled(bool checked)
{
  lineSelected_changed(checked);
}
