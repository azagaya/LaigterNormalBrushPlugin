#include "normalbrushplugin.h"
#include "normalbrushgui.h"
#include <QDebug>

void NormalBrushPlugin::setImages(QImage *normal){
  m_normal = normal;
  normalColor = QColor(127,127,255);
}

void NormalBrushPlugin::mouseMove(const QPoint &oldPos, const QPoint &newPos){
  QImage *normal = m_processor->get_normal();
  QPainter p(normal);
  normalColor = gui->get_normal();
  p.setRenderHint(QPainter::Antialiasing, true);
  p.setPen(QPen(normalColor, radius, Qt::SolidLine, Qt::RoundCap,
                      Qt::MiterJoin));
  p.drawLine(oldPos,newPos);

  QImage *texture = m_processor->get_texture();

  for(int x = 0; x < texture->width(); x++){
    for (int y=0; y < texture->height(); y++){
      if (texture->pixelColor(x,y).alpha() == 0){
        normal->setPixelColor(x,y,QColor(127,127,255));
      }
    }
  }
}

void NormalBrushPlugin::mousePress(const QPoint &pos){

}

void NormalBrushPlugin::mouseRelease(const QPoint &pos){

}

void NormalBrushPlugin::setProcessor(ImageProcessor *processor){
  m_processor = processor;
}

void NormalBrushPlugin::loadGUI(QWidget *parent){
  gui = new NormalBrushGui(parent);
  connect(gui,SIGNAL(radius_changed(int)),this,SLOT(setRadius(int)));
  connect(gui,SIGNAL(normal_changed(QColor)),this,SLOT(set_normal(QColor)));
  radius = 5;
  gui->show();
}

void NormalBrushPlugin::setRadius(int r){
  radius = r;
}

void NormalBrushPlugin::set_normal(QColor n){
  normalColor = n;
}
