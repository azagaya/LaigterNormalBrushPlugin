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

  public:
  explicit NormalBrushGui(QWidget *parent = nullptr);
  ~NormalBrushGui();

  QColor get_normal();

  private:
  Ui::NormalBrushGui *ui;

};

#endif // NORMALBRUSHGUI_H
