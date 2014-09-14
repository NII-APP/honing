#ifndef CGLWIDGET_H
#define CGLWIDGET_H

#include <QGLWidget>
#include <QMatrix4x4>
#include "cparallelepiped.h"
#include <QMenu>

class CGLWidget : public QGLWidget
{
    Q_OBJECT

    int edge;//меньшее из ширины и высоты
    QPair<GLfloat, GLfloat> translate;//Перенос
    QPair<GLfloat, GLfloat> rotate;//Вращение от текущей сессии поворота
    QMatrix4x4 rotateMatrix;//Накопленное вращение
    CParallelepiped sceneSize;//Параллелепипед, в который погружена сцена
    GLfloat zoom;
    QPoint mPPosition;//Последняя отработанная позиция мыши
    QMenu* menu;
    QList<QAction *> sidesMenu;
    QMatrix4x4 rotateBuf;

protected:
    void safelyUpdate() { update(); }//ну там всякие соображения... не придавайте значения... можно менять
    virtual void drawIdantityQuad() { }
    void drawGradienQuad(const QRgb c1, const QRgb c2, const QRgb c3, const QRgb c4);
    void clear();
    void loadRotate();

    void initializeGL();
    virtual void initializeCGL() {}
    void resizeGL(int,int);
    virtual void resizeCGL(int, int) {}
    void paintGL();
    virtual void paintCGL() {}

    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void wheelEvent(QWheelEvent* e);
    void debugSpace(const CParallelepiped &s);

public:
    static void drawAxes();

    CGLWidget(QWidget *parent = 0, const QGLWidget* shareWidget = 0, Qt::WindowFlags f = 0);

    CParallelepiped& scene() { return sceneSize; }
    const CParallelepiped& scene() const { return sceneSize; }


signals:

public slots:
    void transformIdentity();
private slots:
    void menuHandling(QAction* a);
    void menuCatcher();

};

#endif // CGLWIDGET_H
