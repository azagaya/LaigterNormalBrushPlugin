#include "normalbrushplugin.h"
#include "normalbrushgui.h"
#include <algorithm>
#include <QDebug>
#include <QRadialGradient>
#include <QtConcurrent/QtConcurrent>

void NormalBrushPlugin::setImages(QImage *normal){
  m_normal = normal;
  normalColor = QColor(127,127,255);
}

void NormalBrushPlugin::drawAt(QPoint point, QPainter *p){
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
  p->setPen(QPen(brush, radius, Qt::SolidLine, Qt::RoundCap,
                 Qt::MiterJoin));

  p->drawEllipse(point.x()-radius/2,point.y()-radius/2,radius,radius);
}

void NormalBrushPlugin::mouseMove(const QPoint &oldPos, const QPoint &newPos){

  m_processor = *processorPtr;

  if (!selected)
    return;


  int w = m_processor->get_texture()->width();
  int h = m_processor->get_texture()->height();

  bool tilex = m_processor->get_tile_x();
  bool tiley = m_processor->get_tile_y();

  QPoint in(oldPos);
  QPoint fi(newPos);

  int xmin = std::min(in.x(),fi.x());
  int xmax = std::max(in.x(),fi.x());

  int ymin = std::min(in.y(),fi.y());
  int ymax = std::max(in.y(),fi.y());

  if (brushSelected){

    QRect rect = m_processor->get_texture()->rect();

    QPainter p(&auxNormal);
    QImage g(radius,radius,QImage::Format_RGBA8888_Premultiplied);

    if (!linesSelected){
      QRadialGradient gradient(fi, radius);
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

      QPoint point(fi.x()-radius/2, fi.y()-radius/2);

      if (tilex){
        point.setX(point.x() % w);
      }
      if (tiley){
        point.setY(point.y() % h);
      }

      if (tilex){
        if (point.x() + radius >= w){
          drawAt(QPoint(point.x()-w,point.y()),&p);
        } else if (point.x() - radius <= 0){
          drawAt(QPoint(point.x() + w, point.y()), &p);
        }
      }

      if (tiley){
        if (point.y() + radius >= h){
          drawAt(QPoint(point.x(),point.y()-h),&p);
        } else if (point.y() - radius <= 0){
          drawAt(QPoint(point.x(), point.y()+h), &p);
        }
      }

      drawAt(point, &p);
    }else{

      QPainterPath path;
      path.moveTo(in);
      path.lineTo(fi);
      qreal length = path.length();
      qreal pos = 0;


      while (pos < length) {
        qreal percent = path.percentAtLength(pos);
        QPoint point = path.pointAtPercent(percent).toPoint();

        if (tilex){
          point.setX(point.x() % w);
          xmin = std::min(xmin,point.x());
          xmax = std::max(xmax,point.x());
        }
        if (tiley){
          point.setY(point.y() % h);
          ymin = std::min(ymin,point.y());
          ymax = std::max(ymax,point.y());
        }

        if (tilex){
          if (point.x() + radius >= w){
            drawAt(QPoint(point.x()-w,point.y()),&p);
            xmax = w;
            xmin = 0;
          } else if (point.x() - radius <= 0){
            drawAt(QPoint(point.x() + w, point.y()), &p);
            xmax = w;
            xmin = 0;

          }
        }

        if (tiley){
          if (point.y() + radius >= h){
            drawAt(QPoint(point.x(),point.y()-h),&p);
            ymax = h;
            ymin = 0;
          } else if (point.y() - radius <= 0){
            drawAt(QPoint(point.x(), point.y()+h), &p);
            ymax = h;
            ymin = 0;
          }
        }

        drawAt(point,&p);
        pos += radius/4.0;
      }
    }

    QImage *overlay = m_processor->get_normal_overlay();

    QPoint topLeft;
    QPoint botRight;

    topLeft = QPoint(xmin,ymin);
    botRight = QPoint(xmax,ymax);

    xmin -= radius;
    ymin -= radius;
    xmax += radius;
    ymax += radius;

    ymin = ymin < 0 ? 0 : ymin;
    ymin = ymin > overlay->height() ? overlay->height() : ymin;
    ymax = ymax < 0 ? 0 : ymax;
    ymax = ymax > overlay->height() ? overlay->height() : ymax;

    xmin = xmin < 0 ? 0 : xmin;
    xmin = xmin > overlay->width() ? overlay->width() : xmin;
    xmax = xmax < 0 ? 0 : xmax;
    xmax = xmax > overlay->width() ? overlay->width() : xmax;

    for (int x = xmin; x < xmax; x++){
      for (int y =ymin; y < ymax; y++){
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
    p.drawLine(in,fi);

    for (int x=0; x < overlay->width(); x++){
      for (int y=0; y < overlay->height(); y++){
        if (auxNormal.pixelColor(x,y).alphaF() != 0){
          overlay->setPixelColor(x,y,QColor(0,0,0,0));
        }
      }
    }

  }
  //}
  //  }

  xmin = std::min(in.x(),fi.x());
  xmax = std::max(in.x(),fi.x());

  ymin = std::min(in.y(),fi.y());
  ymax = std::max(in.y(),fi.y());

  QRect r(QPoint(xmin-radius,ymin-radius),QPoint(xmax+radius,ymax+radius));

  QtConcurrent::run(m_processor,&ImageProcessor::generate_normal_map,false,false,false,r);
}

void NormalBrushPlugin::mousePress(const QPoint &pos){
  m_processor = *processorPtr;
  QImage *overlay = m_processor->get_normal_overlay();
  oldNormal = QImage(overlay->width(),overlay->height(),QImage::Format_RGBA8888_Premultiplied);
  oldNormal = *overlay;
  auxNormal = QImage(oldNormal.width(), oldNormal.height(), QImage::Format_RGBA8888_Premultiplied);
  auxNormal.fill(QColor(0,0,0,0));
}

void NormalBrushPlugin::mouseRelease(const QPoint &pos){

}

void NormalBrushPlugin::setProcessor(ImageProcessor **processor){
  processorPtr = processor;
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
  if (e && !selected) set_selected(e);
  else if (!e && !brushSelected) set_selected(e);
}

void NormalBrushPlugin::set_brushSelected(bool b){
  brushSelected = b;
  if (b && !selected) set_selected(b);
  else if (!b && !eraserSelected) set_selected(b);
}

QIcon NormalBrushPlugin::getIcon(){
  return QIcon(QPixmap(":/icons/icon.png"));
}

QString NormalBrushPlugin::getName(){
  return m_name;
}

bool NormalBrushPlugin::get_selected(){
  return selected;
}

void NormalBrushPlugin::set_selected(bool s){
  selected = s;
  if (!s){
    gui->unselect_all();
  }
}
