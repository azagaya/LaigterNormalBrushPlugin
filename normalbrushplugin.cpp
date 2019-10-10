#include "normalbrushplugin.h"
#include "normalbrushgui.h"
#include <QDebug>
#include <QRadialGradient>

void NormalBrushPlugin::setImages(QImage *normal){
  m_normal = normal;
  normalColor = QColor(127,127,255);
}

void NormalBrushPlugin::mouseMove(const QPoint &oldPos, const QPoint &newPos){

  if (!selected)
    return;

  if (brushSelected){

    QRect rect = m_processor->get_texture()->rect();
    if (!rect.contains(oldPos) && !rect.contains(newPos))
      return;

    QPainter p(&auxNormal);
    QImage g(radius,radius,QImage::Format_RGBA8888_Premultiplied);

    if (!linesSelected){

      QRadialGradient gradient(newPos, radius);
      normalColor = gui->get_normal();
      gradient.setColorAt(0,normalColor);
      normalColor.setAlphaF(pow(0.5+hardness*0.5,2));
      gradient.setColorAt(hardness,normalColor);
      if (hardness != 1){
        normalColor.setAlphaF(0);
        gradient.setColorAt(1,normalColor);
      }

      QBrush brush(gradient);
      p.setRenderHint(QPainter::Antialiasing, true);
      if (brushSelected)
        p.setPen(QPen(brush, radius, Qt::SolidLine, Qt::RoundCap,
                      Qt::MiterJoin));

      p.drawEllipse(newPos.x()-radius/2,newPos.y()-radius/2,radius,radius);
    }else{

      QPainterPath path;
      path.moveTo(oldPos);
      path.lineTo(newPos);
      qreal length = path.length();
      qreal pos = radius/4;

      while (pos < length) {
        qreal percent = path.percentAtLength(pos);
        QPoint point = path.pointAtPercent(percent).toPoint();

        QRadialGradient gradient(point, radius);
        normalColor = gui->get_normal();
        gradient.setColorAt(0,normalColor);
        normalColor.setAlphaF(pow(0.5+hardness*0.5,2));
        gradient.setColorAt(hardness,normalColor);
        if (hardness != 1){
          normalColor.setAlphaF(0);
          gradient.setColorAt(1,normalColor);
        }

        QBrush brush(gradient);
        p.setPen(QPen(brush, radius, Qt::SolidLine, Qt::RoundCap,
                      Qt::MiterJoin));

        p.drawEllipse(point.x()-radius/2,point.y()-radius/2,radius,radius);
        pos += radius/4.0;
      }
    }

    QImage *overlay = m_processor->get_normal_overlay();

    QPoint topLeft;
    QPoint botRight;

    if (oldPos.x() > newPos.x()){
      botRight.setX(oldPos.x());
      topLeft.setX(newPos.x());
    } else {
      botRight.setX(newPos.x());
      topLeft.setX(oldPos.x());
    }

    if (oldPos.y() > newPos.y()){
      botRight.setY(oldPos.y());
      topLeft.setY(newPos.y());
    } else {
      botRight.setY(newPos.y());
      topLeft.setY(oldPos.y());
    }

    int xi,xf,yi,yf;

    rect = QRect(topLeft-QPoint(radius,radius),botRight+QPoint(radius,radius));
    rect.getCoords(&xi,&yi,&xf,&yf);

    yi = yi < 0 ? 0 : yi;
    yf = yf > overlay->height() ? overlay->height() : yf;

    xi = xi < 0 ? 0 : xi;
    xf = xf > overlay->height() ? overlay->width() : xf;

    for (int x = xi; x < xf; x++){
      for (int y = yi; y < yf; y++){
        QColor oldColor = oldNormal.pixelColor(x,y);
        QColor auxColor = auxNormal.pixelColor(x,y);
        QColor newColor(0,0,0,0);

        if (auxColor.alphaF() <= 0.05 || m_processor->get_texture()->pixelColor(x,y).alphaF() == 0){
          newColor = oldColor;
        } else {

          float outA = alpha*auxColor.alphaF()+oldColor.alphaF()*(1-alpha*auxColor.alphaF());
          int r = auxColor.red()*alpha*auxColor.alphaF()+oldColor.red()*oldColor.alphaF()*(1-alpha*auxColor.alphaF ());

          int g = auxColor.green()*alpha*auxColor.alphaF()+oldColor.green()*oldColor.alphaF()*(1-alpha*auxColor.alphaF());

          int b = auxColor.blue()*alpha*auxColor.alphaF()+oldColor.blue()*oldColor.alphaF()*(1-alpha*auxColor.alphaF());

          newColor = QColor(r,g,b);
          newColor.setAlphaF(outA);

        }


        overlay->setPixelColor(x,y,newColor);
      }
    }
  } else {
    QImage *overlay = m_processor->get_normal_overlay();
    QPainter p(&auxNormal);
    QPen pen(QColor(1,1,1,1));
    pen.setWidth(2*radius);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::MiterJoin);
    p.setPen(pen);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.drawLine(oldPos,newPos);

    for (int x=0; x < overlay->width(); x++){
      for (int y=0; y < overlay->height(); y++){
        if (auxNormal.pixelColor(x,y).alphaF() != 0){
          overlay->setPixelColor(x,y,QColor(0,0,0,0));
        }
      }
    }

  }
  m_processor->generate_normal_map();
}

void NormalBrushPlugin::mousePress(const QPoint &pos){
  QImage *overlay = m_processor->get_normal_overlay();
  oldNormal = QImage(overlay->width(),overlay->height(),QImage::Format_RGBA8888_Premultiplied);
  oldNormal = *overlay;
  auxNormal = QImage(oldNormal.width(), oldNormal.height(), QImage::Format_RGBA8888_Premultiplied);
  auxNormal.fill(QColor(0,0,0,0));
}

void NormalBrushPlugin::mouseRelease(const QPoint &pos){

}

void NormalBrushPlugin::setProcessor(ImageProcessor *processor){
  m_processor = processor;
}

QWidget *NormalBrushPlugin::loadGUI(QWidget *parent){
  gui = new NormalBrushGui(parent);
  connect(gui,SIGNAL(radius_changed(int)),this,SLOT(set_radius(int)));
  connect(gui,SIGNAL(normal_changed(QColor)),this,SLOT(set_normal(QColor)));
  connect(gui,SIGNAL(mix_changed(int)),this,SLOT(set_mix(int)));
  connect(gui,SIGNAL(hardness_changed(int)),this,SLOT(set_hardness(int)));
  connect(gui,SIGNAL(lineSelected_changed(bool)),this,SLOT(set_lineSelected(bool)));
  connect(gui,SIGNAL(brushSelected_changed(bool)),this,SLOT(set_brushSelected(bool)));
  connect(gui,SIGNAL(eraserSelected_changed(bool)),this,SLOT(set_eraserSelected(bool)));
  radius = 10;
  //gui->show();
  return gui;
}

void NormalBrushPlugin::set_radius(int r){
  radius = r;
}

void NormalBrushPlugin::set_hardness(int h){
  hardness = h/100.0;
}

void NormalBrushPlugin::set_normal(QColor n){
  normalColor = n;
}

void NormalBrushPlugin::set_mix(int m){
  alpha = m/100.0;
}

void NormalBrushPlugin::set_lineSelected(bool l){
  linesSelected = l;
}

void NormalBrushPlugin::set_eraserSelected(bool e){
  eraserSelected = e;
}

void NormalBrushPlugin::set_brushSelected(bool b){
  brushSelected = b;
}

QIcon NormalBrushPlugin::getIcon(){
  return QIcon(QPixmap(":/icons/icon.png"));
}

QString NormalBrushPlugin::getName(){
  return m_name;
}
