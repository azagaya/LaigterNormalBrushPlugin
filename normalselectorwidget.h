#ifndef NORMALSELECTORWIDGET_H
#define NORMALSELECTORWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>

class NormalSelectorWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
  public:
  NormalSelectorWidget(QWidget *parent = nullptr);
  QColor get_normal();
  private:
  void renderSphere();
  QOpenGLVertexArrayObject sphereVAO, planeVAO;
  QOpenGLBuffer vbo, ebo, planeVbo, planeEbo;
  unsigned int indexCount;
  QOpenGLShaderProgram sphere, plane;
  QColor c = QColor(127,127,255);
  QVector2D p = QVector2D(0,0);
  QImage rendered;
  bool updated = false;

  protected:
  void initializeGL() override ;
  void paintGL() override;
  void resizeGL(int width, int height) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;

  private:
      QOpenGLContext *context;
};

#endif // NORMALSELECTORWIDGET_H
