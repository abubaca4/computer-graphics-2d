#ifndef LAB1_H
#define LAB1_H

#include <QMainWindow>
#include <QColor>
#include <QMouseEvent>
#include <QRandomGenerator>
#include <QPainter>
#include <QVector>
#include <QPoint>
#include <QtMath>
#include <QPair>

QT_BEGIN_NAMESPACE
namespace Ui { class Lab1; }
QT_END_NAMESPACE

class Lab1 : public QMainWindow
{
    Q_OBJECT

public:
    Lab1(QWidget *parent = nullptr);
    ~Lab1();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

private:
    Ui::Lab1 *ui;

    struct some_points{
        QVector<QPoint> points;
        bool check_point(const QPoint &point) const;
    };
    class figure{
    public:
        void add_point(const int &x, const int &y);
        void set_collor(const QColor &collor);
        void set_angle(const qreal &angle);
        void set_scale(const qreal &scale);
        void paint(QPainter &Painter, const QVector<some_points> &triangles) const;
        bool check_point(const QPoint &point) const;
        void move(const int &x, const int &y);
        const QPoint& get_center() const;
        qsizetype points_count() const;
        QVector<some_points> triangulate() const;

    private:
        void renew_center();
        QColor collor;
        QPoint center;
        qreal angle = 0.0;
        qreal scale = 1.0;
        QVector<QPoint> points, origin_points;
    };
    static qreal rotate_angle(const QPoint& start, const QPoint& center, const QPoint& current);
    static qreal D_b_points(const QPoint& p1, const QPoint& p2);
    static QVector<QPair<QPoint, QPoint>> cyrus_beck(QPair<QPoint, QPoint> line, const some_points &shape);
    QVector<figure> figures;
    enum mode{
        add,
        move,
        rotate,
        scale
    };
    mode current_mode = add;
    figure on_edit;

    bool is_moving = false;
    QPoint moving_start_point;
    figure *moving_figure;

    bool is_rotating = false;
    QPoint rotating_start_point;
    figure *rotating_figure;

    bool is_scaling = false;
    QPoint scaling_start_point;
    figure *scaling_figure;
};
#endif // LAB1_H
