#ifndef NORMALBRUSHPLUGIN_H
#define NORMALBRUSHPLUGIN_H

#include <QObject>
#include <QWidget>
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
  void loadGUI(QWidget *parent = nullptr) override;
  void setProcessor(ImageProcessor *processor) override;

  public slots:
  void setRadius(int r);
  void set_normal(QColor n);

  private:
  QImage *m_normal;
  int radius;
  QColor normalColor;
  NormalBrushGui *gui;
  ImageProcessor *m_processor;

};

#endif // NORMALBRUSHPLUGIN_H
