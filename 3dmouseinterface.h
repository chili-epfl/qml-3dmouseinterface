#ifndef MOUSEINTERFACE3D_H
#define MOUSEINTERFACE3D_H

#include <QQuickItem>
#include <Qt3DCore>
#include <Qt3DRenderer/QMesh>
#include <Qt3DCore/QComponentList>
#include <Qt3DCore/QCamera>

class MouseInterface3D : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(MouseInterface3D)
    Q_PROPERTY(Qt3D::QEntity* scene3D WRITE setScene3D)
    Q_PROPERTY(Qt3D::QCamera* camera WRITE setCamera)

public:
    MouseInterface3D(QQuickItem *parent = 0);
    ~MouseInterface3D();
    void setScene3D(Qt3D::QEntity* scene3D);
    void setCamera(Qt3D::QCamera* camera);

public slots:
    void select(QVector2D mouseXYNormalized);

private:
    bool intersects(const Qt3D::QRay3D ray, const QVector3D ray_dir_inv, const int sign[], const QPair<QVector3D,QVector3D> aabb,qreal& tnear);

    //Sorted list of entities.
    QSet<Qt3D::QEntity*> m_selectedItems;
    Qt3D::QEntity* m_sceneroot;
    Qt3D::QCamera* m_camera;


};

#endif // MOUSEINTERFACE3D_H
