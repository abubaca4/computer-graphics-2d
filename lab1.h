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
#include <QDebug>

#include <algorithm>
#include <functional>

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
    void on_pushButton_5_clicked();
    void on_checkBox_stateChanged(int arg1);

private:
    Ui::Lab1 *ui;

    struct some_points{
        QVector<QPoint> points;
        bool check_point(const QPoint &point) const;
        static bool compare_points_of_center(const QPoint &p1, const QPoint &p2, const QPoint &center);
        void sort_poits_clock();
        void paint(QPainter &Painter,const QColor &collor);
    };
    class figure{
    public:
        void add_point(const int &x, const int &y);
        void set_collor(const QColor &collor);
        void set_angle(const qreal &angle);
        void set_scale(const qreal &scale);
        void paint(QPainter &Painter, const QVector<some_points> &triangles) const;
        void paint_triangles(QPainter &Painter);
        bool check_point(const QPoint &point) const;
        void move(const int &x, const int &y);
        const QPoint& get_center() const;
        qsizetype points_count() const;
        QVector<some_points> triangulate() const;        

    private:
        void renew_center();

        QPoint center;
        qreal angle = 0.0;
        qreal scale = 1.0;
        QVector<QPoint> points, origin_points;
        QColor collor;
    };
    static qreal rotate_angle(const QPoint& start, const QPoint& center, const QPoint& current);
    static qreal D_b_points(const QPoint& p1, const QPoint& p2);
    static bool check_point_inside(const QVector<QPoint> &points, const QPoint &point);
    static QPoint find_center(const QVector<QPoint> &points);
    static QVector<QPair<QPoint, QPoint>> cyrus_beck(const QPair<QPoint, QPoint> &line, const some_points &shape);
    QVector<figure> figures;
    enum mode{
        add,
        move,
        rotate,
        scale,
        front
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
