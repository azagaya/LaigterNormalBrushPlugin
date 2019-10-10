#ifndef NORMALBRUSHPLUGIN_H
#define NORMALBRUSHPLUGIN_H

#include <QObject>
#include <QWidget>
#include <QIcon>

#include <brushinterface.h>
#include <imageprocessor.h>
#include "normalbrushgui.h"
#include "normalselectorwidget.h"
class NormalBrushPlugin : public QWidget, public BrushInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.azagaya.laigter.plugins.BrushInterface")
  Q_INTERFACES(BrushInterface)

  public:
  void setImages(QImage *normal) override;
  void mousePress(const QPoint &pos) override;
  void mouseMove(const QPoint &oldPos, const QPoint &newPos) override;
  void mouseRelease(const QPoint &pos) override;
  QWidget *loadGUI(QWidget *parent = nullptr) override;
  void setProcessor(ImageProcessor *processor) override;
  QIcon getIcon() override;
  QString getName() override;

  public slots:
  void set_radius(int r);
  void set_normal(QColor n);
  void set_mix(int m);
  void set_hardness(int h);
  void set_lineSelected(bool l);
  void set_eraserSelected(bool e);
  void set_brushSelected(bool b);

  private:
  QImage *m_normal, auxNormal, oldNormal;
  int radius;
  QColor normalColor;
  NormalBrushGui *gui;
  ImageProcessor *m_processor;
  float alpha = 0.8;
  float hardness = 0.8;

  bool linesSelected = true, brushSelected = true, eraserSelected = false, selected = true;
  QString m_name = tr("Normal Brush Plugin");
};

#endif // NORMALBRUSHPLUGIN_H
