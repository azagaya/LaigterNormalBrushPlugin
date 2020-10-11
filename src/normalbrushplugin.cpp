#include "src/normalbrushplugin.h"
#include "src/normalbrushgui.h"
#include <QDebug>
#include <QPainterPath>
#include <QRadialGradient>
#include <QtConcurrent/QtConcurrent>
#include <algorithm>

void NormalBrushPlugin::updateOverlay(int xmin, int xmax, int ymin, int ymax) {
  QImage overlay = m_processor->get_normal_overlay();

  QPoint topLeft;
  QPoint botRight;

  topLeft = QPoint(xmin, ymin);
  botRight = QPoint(xmax, ymax);

  QRect imageRect = diffuse.rect();
  bool tile_x = m_processor->get_tile_x();
  bool tile_y = m_processor->get_tile_y();

  int w = diffuse.width();
  int h = diffuse.height();

  for (int x = xmin; x < xmax; x++) {
    for (int y = ymin; y < ymax; y++) {

      int ix = x;
      int iy = y;

      if (!imageRect.contains(QPoint(x, y))) {
        if (!tile_x && !tile_y) {
          continue;
        }
        if (tile_x) {
          ix = WrapCoordinate(ix, w);
        }
        if (tile_y) {
          iy = WrapCoordinate(iy, h);
        }
      }

      QColor oldColor = oldNormal.pixelColor(ix, iy);
      QColor auxColor = auxNormal.pixelColor(ix, iy);
      QColor newColor(0, 0, 0, 0);

      if (auxColor.alphaF() <= 1e-6 ||
          diffuse.pixelColor(ix, iy).alphaF() == 0) {
        newColor = oldColor;
      } else {

        float outA = alpha * auxColor.alphaF() +
                     oldColor.alphaF() * (1 - alpha * auxColor.alphaF());
        int r = auxColor.red() * alpha * auxColor.alphaF() / outA +
                oldColor.red() * oldColor.alphaF() *
                    (1 - alpha * auxColor.alphaF()) / outA;

        int g = auxColor.green() * alpha * auxColor.alphaF() / outA +
                oldColor.green() * oldColor.alphaF() *
                    (1 - alpha * auxColor.alphaF()) / outA;

        int b = auxColor.blue() * alpha * auxColor.alphaF() / outA +
                oldColor.blue() * oldColor.alphaF() *
                    (1 - alpha * auxColor.alphaF()) / outA;

        newColor = QColor(r, g, b);
        newColor.setAlphaF(outA);
      }
      overlay.setPixelColor(ix, iy, newColor);
    }
  }
  m_processor->set_normal_overlay(overlay);
}

void NormalBrushPlugin::drawAt(QPoint point, QPainter *p, float alpha_mod,
                               bool tile_x, bool tile_y) {

  int w = p->device()->width();
  int h = p->device()->height();

  QList<QPoint> pointList;

  if (tile_x) {
    point.setX(WrapCoordinate(point.x(), w));
  }

  if (tile_y) {
    point.setY(WrapCoordinate(point.y(), h));
  }

  if (tile_x) {

    pointList.append(point - QPoint(w, 0));
    pointList.append(point + QPoint(w, 0));
  }

  if (tile_y) {
    pointList.append(point - QPoint(0, h));
    pointList.append(point + QPoint(0, h));
  }

  pointList.append(point);

  foreach (QPoint point, pointList) {
    QRadialGradient gradient(point, radius);
    normalColor = gui->get_normal();
    normalColor.setAlphaF(normalColor.alphaF() * alpha_mod);

    gradient.setColorAt(0, normalColor);
    normalColor.setAlphaF(hardness * alpha_mod);
    gradient.setColorAt(pow(hardness * radius, 2) / pow(radius, 2),
                        normalColor);
    normalColor.setAlphaF(alpha_mod * floor(hardness));
    gradient.setColorAt(1, normalColor);

    QBrush brush(gradient);
    p->setPen(QPen(brush, radius, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));

    p->drawEllipse(point.x() - radius / 2, point.y() - radius / 2, radius,
                   radius);
  }
}

void NormalBrushPlugin::mouseMove(const QPoint &oldPos, const QPoint &newPos) {
  m_processor = *processorPtr;

  if (!selected)
    return;

  QPoint oldP = WorldToLocal(oldPos);
  QPoint newP = WorldToLocal(newPos);

  QPoint in(oldP);
  QPoint fi(newP);

  int xmin = std::min(in.x(), fi.x());
  int xmax = std::max(in.x(), fi.x());

  int ymin = std::min(in.y(), fi.y());
  int ymax = std::max(in.y(), fi.y());

  bool tile_x = m_processor->get_tile_x();
  bool tile_y = m_processor->get_tile_y();

  if (brushSelected) {

    QPainter p(&auxNormal);

    if (!linesSelected) {
      QRadialGradient gradient(fi, radius);
      normalColor = gui->get_normal();
      gradient.setColorAt(0, normalColor);
      normalColor.setAlphaF(pow(0.5 + hardness * 0.5, 2));
      gradient.setColorAt(hardness, normalColor);
      if (hardness != 1) {
        normalColor.setAlphaF(0);
        gradient.setColorAt(1, normalColor);
      }

      QBrush brush(gradient);
      p.setRenderHint(QPainter::Antialiasing, false);
      if (brushSelected)
        p.setPen(
            QPen(brush, radius, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
      QPoint point(fi.x(), fi.y());
      drawAt(point, &p, 1.0, tile_x, tile_y);
    } else {
      QPainterPath path;
      path.moveTo(in);
      path.lineTo(fi);
      qreal length = path.length();
      qreal pos = 0;

      while (pos < length) {
        qreal percent = path.percentAtLength(pos);
        QPoint point = path.pointAtPercent(percent).toPoint();
        drawAt(point, &p, 1.0, tile_x, tile_y);
        pos += radius / 4.0;
      }
    }

    updateOverlay(xmin - radius, xmax + radius, ymin - radius, ymax + radius);

  } else {
    QImage overlay = m_processor->get_normal_overlay();
    QPainter p(&overlay);
    p.setCompositionMode(QPainter::CompositionMode_Source);
    QPen pen(QColor(0, 0, 0, 0));
    pen.setWidth(2 * radius);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::MiterJoin);
    p.setPen(pen);
    p.setRenderHint(QPainter::Antialiasing, false);
    p.drawLine(in, fi);

    m_processor->set_normal_overlay(overlay);
  }

  xmin = std::min(in.x(), fi.x());
  xmax = std::max(in.x(), fi.x());

  ymin = std::min(in.y(), fi.y());
  ymax = std::max(in.y(), fi.y());

  QRect r(QPoint(xmin - radius, ymin - radius),
          QPoint(xmax + radius, ymax + radius));

  QtConcurrent::run(m_processor, &ImageProcessor::generate_normal_map, false,
                    false, false, r);
}

void NormalBrushPlugin::mousePress(const QPoint &pos) {
  m_processor = *processorPtr;
  QImage overlay = m_processor->get_normal_overlay();
  oldNormal = QImage(overlay.width(), overlay.height(),
                     QImage::Format_RGBA8888_Premultiplied);
  oldNormal = overlay;
  auxNormal = QImage(oldNormal.width(), oldNormal.height(),
                     QImage::Format_RGBA8888_Premultiplied);
  auxNormal.fill(QColor(0, 0, 0, 0));

  bool tile_x = m_processor->get_tile_x();
  bool tile_y = m_processor->get_tile_y();

  QPoint newP = WorldToLocal(pos);
  /* Draw the point */

  QPoint fi(newP);
  if (brushSelected) {
    QPainter p(&auxNormal);

    QRadialGradient gradient(fi, radius);
    normalColor = gui->get_normal();
    gradient.setColorAt(0, normalColor);
    normalColor.setAlphaF(pow(0.5 + hardness * 0.5, 2));
    gradient.setColorAt(hardness, normalColor);
    if (hardness != 1) {
      normalColor.setAlphaF(0);
      gradient.setColorAt(1, normalColor);
    }

    QBrush brush(gradient);
    p.setRenderHint(QPainter::Antialiasing, false);
    if (brushSelected)
      p.setPen(QPen(brush, radius, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));

    QPoint point(fi.x(), fi.y());

    drawAt(point, &p, 1.0, tile_x, tile_y);
    updateOverlay(fi.x() - radius, fi.x() + radius, fi.y() - radius,
                  fi.y() + radius);
  } else {
    QImage overlay = m_processor->get_normal_overlay();
    QPainter p(&overlay);
    p.setCompositionMode(QPainter::CompositionMode_Source);
    QPen pen(QColor(0, 0, 0, 0));
    pen.setWidth(2 * radius);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::MiterJoin);
    p.setPen(pen);
    p.setRenderHint(QPainter::Antialiasing, false);
    p.drawPoint(fi);
    m_processor->set_normal_overlay(overlay);
  }
  QRect r(QPoint(fi.x() - radius, fi.y() - radius),
          QPoint(fi.x() + radius, fi.y() + radius));
  QtConcurrent::run(m_processor, &ImageProcessor::generate_normal_map, false,
                    false, false, r);
}

void NormalBrushPlugin::mouseRelease(const QPoint &pos) {}

void NormalBrushPlugin::setProcessor(ImageProcessor **processor) {

  processorPtr = processor;
  (*processor)->get_current_frame()->get_image(TextureTypes::Diffuse, &diffuse);
}

QWidget *NormalBrushPlugin::loadGUI(QWidget *parent) {

  gui = new NormalBrushGui(parent);
  connect(gui, SIGNAL(radius_changed(int)), this, SLOT(set_radius(int)));
  connect(gui, SIGNAL(normal_changed(QColor)), this, SLOT(set_normal(QColor)));
  connect(gui, SIGNAL(mix_changed(int)), this, SLOT(set_mix(int)));
  connect(gui, SIGNAL(hardness_changed(int)), this, SLOT(set_hardness(int)));
  connect(gui, SIGNAL(lineSelected_changed(bool)), this,
          SLOT(set_lineSelected(bool)));
  connect(gui, SIGNAL(brushSelected_changed(bool)), this,
          SLOT(set_brushSelected(bool)));
  connect(gui, SIGNAL(eraserSelected_changed(bool)), this,
          SLOT(set_eraserSelected(bool)));
  radius = 10;
  updateBrushSprite();
  return gui;
}

void NormalBrushPlugin::set_radius(int r) {
  radius = r;
  updateBrushSprite();
}

void NormalBrushPlugin::set_hardness(int h) {
  hardness = h / 100.0;
  updateBrushSprite();
}

void NormalBrushPlugin::set_normal(QColor n) {
  normalColor = n;
  updateBrushSprite();
}

void NormalBrushPlugin::set_mix(int m) {
  alpha = m / 100.0;
  updateBrushSprite();
}

void NormalBrushPlugin::set_lineSelected(bool l) { linesSelected = l; }

void NormalBrushPlugin::set_eraserSelected(bool e) {
  eraserSelected = e;
  if (e && !selected)
    set_selected(e);
  else if (!e && !brushSelected)
    set_selected(e);
}

void NormalBrushPlugin::set_brushSelected(bool b) {
  brushSelected = b;
  if (b && !selected)
    set_selected(b);
  else if (!b && !eraserSelected)
    set_selected(b);
}

QString NormalBrushPlugin::getIcon() { return "brush-normal"; }

QString NormalBrushPlugin::getName() { return m_name; }

bool NormalBrushPlugin::get_selected() { return selected; }

void NormalBrushPlugin::set_selected(bool s) {
  selected = s;
  if (!s) {
    gui->unselect_all();
  } else {
    selected_changed(this);
  }
}

QImage NormalBrushPlugin::getBrushSprite() { return brushSprite; }

void NormalBrushPlugin::updateBrushSprite() {
  brushSprite = QImage(2 * radius, 2 * radius, QImage::Format_RGBA8888);
  brushSprite.fill(0.0);
  QPainter p(&brushSprite);
  drawAt(QPoint(radius, radius), &p, alpha);
}
QObject *NormalBrushPlugin::getObject() { return this; }

QPoint NormalBrushPlugin::WorldToLocal(QPoint world) {
  int w = diffuse.width();
  int h = diffuse.height();

  QPoint origin = m_processor->get_position()->toPoint();

  QPoint local = world + QPoint(0.5 * w, -0.5 * h) - origin;
  local.setY(-local.y());

  return local;
}

int NormalBrushPlugin::WrapCoordinate(int coord, int interval) {
  coord %= interval;
  if (coord < 0) {
    coord += interval;
  }
  return coord;
}
