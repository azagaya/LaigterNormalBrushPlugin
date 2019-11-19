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
  bool get_selected() override;
  void set_selected(bool s) override;
  QWidget *loadGUI(QWidget *parent = nullptr) override;
  void setProcessor(ImageProcessor **processor) override;
  QIcon getIcon() override;
  QString getName() override;
  QImage getBrushSprite() override;
  void drawAt(QPoint point, QPainter *p, float alpha_mod = 1.0);
  void updateBrushSprite();

  public slots:
  void set_radius(int r);
  void set_normal(QColor n);
  void set_mix(int m);
  void set_hardness(int h);
  void set_lineSelected(bool l);
  void set_eraserSelected(bool e);
  void set_brushSelected(bool b);

  private:
  QImage *m_normal, auxNormal, oldNormal, brushSprite;
  int radius;
  QColor normalColor;
  NormalBrushGui *gui;
  ImageProcessor **processorPtr;
  ImageProcessor *m_processor;
  float alpha = 0.8;
  float hardness = 0.8;

  bool linesSelected = true, brushSelected = false, eraserSelected = false, selected = false;
  QString m_name = tr("Normal Brush Plugin");
};

#endif // NORMALBRUSHPLUGIN_H
