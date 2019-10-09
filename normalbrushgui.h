#ifndef NORMALBRUSHGUI_H
#define NORMALBRUSHGUI_H

#include <QWidget>
#include "normalselectorwidget.h"

namespace Ui {
class NormalBrushGui;
}

class NormalBrushGui : public QWidget
{
  Q_OBJECT

  signals:
      void radius_changed(int r);
      void normal_changed(QColor n);
      void mix_changed(int m);
      void hardness_changed(int h);
      void lineSelected_changed(bool l);
      void brushSelected_changed(bool b);
      void eraserSelected_changed(bool e);
      void selected_changed(bool s);

  public:
  explicit NormalBrushGui(QWidget *parent = nullptr);
  ~NormalBrushGui();

  QColor get_normal();

  private slots:
  void on_buttonBrush_toggled(bool checked);

  void on_buttonEraser_toggled(bool checked);

  void on_radioLines_toggled(bool checked);

  private:
  Ui::NormalBrushGui *ui;

};

#endif // NORMALBRUSHGUI_H
