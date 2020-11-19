#ifndef NORMALBRUSHPLUGIN_H
#define NORMALBRUSHPLUGIN_H

#include <QIcon>
#include <QObject>
#include <QWidget>

#include "src/normalbrushgui.h"
#include "src/normalselectorwidget.h"
#include <brush_interface.h>
#include <image_processor.h>
class NormalBrushPlugin : public QObject, public BrushInterface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.azagaya.laigter.plugins.BrushInterface" FILE
                        "metadata.json")
  Q_INTERFACES(BrushInterface)

public:
  void mousePress(const QPoint &pos) override;
  void mouseMove(const QPoint &oldPos, const QPoint &newPos) override;
  void mouseRelease(const QPoint &pos) override;
  void setPressure(float pressure) override;
  bool get_selected() override;
  void set_selected(bool s) override;
  QWidget *loadGUI(QWidget *parent = nullptr) override;
  void setProcessor(ImageProcessor **processor) override;
  QString getIcon() override;
  QString getName() override;
  QImage getBrushSprite() override;
  void drawAt(QPoint point, QPainter *p, float alpha_mod = 1.0,
              bool tile_x = false, bool tile_y = false);
  void updateOverlay(int xmin, int xmax, int ymin, int ymax);
  void updateBrushSprite();
  QObject *getObject() override;
  QPoint WorldToLocal(QPoint world);
  int WrapCoordinate(int coord, int interval);

signals:
  void selected_changed(BrushInterface *brush);

public slots:
  void set_radius(int r);
  void set_base_radius(int r);
  void set_normal(QColor n);
  void set_mix(int m);
  void set_hardness(int h);
  void set_lineSelected(bool l);
  void set_eraserSelected(bool e);
  void set_brushSelected(bool b);

private:
  QImage *m_normal, auxNormal, oldNormal, brushSprite, diffuse;
  int radius, base_radius;
  QColor normalColor;
  NormalBrushGui *gui;
  ImageProcessor **processorPtr;
  ImageProcessor *m_processor;
  float alpha = 0.8;
  float hardness = 0.8;
  float pressure = 1.0;

  bool linesSelected = true, brushSelected = false, eraserSelected = false,
       selected = false;
  QString m_name = tr("Normal Brush Plugin");
};

#endif // NORMALBRUSHPLUGIN_H
