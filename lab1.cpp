#include "lab1.h"
#include "ui_lab1.h"

Lab1::Lab1(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Lab1)
{
    ui->setupUi(this);
}

Lab1::~Lab1()
{
    delete ui;
}

void Lab1::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QVector<some_points> triangles;
    on_edit.paint(painter, triangles);
    for (auto &i: figures)
    {
        i.paint(painter, triangles);
        triangles += i.triangulate();
    } 
}

void Lab1::mousePressEvent(QMouseEvent *event)
{
    switch (current_mode) {
    case add:
        switch (event->button()) {
        case Qt::LeftButton:
            if (on_edit.points_count() == 0)
            {
                on_edit.set_collor(QColor(QRandomGenerator::global()->generate()%255, QRandomGenerator::global()->generate()%255, QRandomGenerator::global()->generate()%255));
            }
            on_edit.add_point(event->pos().x(), event->pos().y());
            break;

        case Qt::RightButton:
            if (on_edit.points_count() > 2)
            {
                on_edit.move(0, 0);
                figures.push_back(on_edit);
                on_edit = figure();
            }
            break;

        default:
            break;
        }
        break;

    case move:
        switch (event->button()) {
        case Qt::LeftButton:
            for (auto &i: figures) {
                if (i.check_point(event->pos()))
                {
                    is_moving = true;
                    moving_start_point = event->pos();
                    moving_figure = &i;
                    break;
                }
            }
            break;

        default:
            break;
        }
        break;

    case rotate:
        switch (event->button()) {
        case Qt::LeftButton:
            for (auto &i: figures) {
                if (i.check_point(event->pos()))
                {
                    is_rotating = true;
                    rotating_start_point = event->pos();
                    rotating_figure = &i;
                    break;
                }
            }
            break;

        default:
            break;
        }
        break;

    case scale:
        switch (event->button()) {
        case Qt::LeftButton:
            for (auto &i: figures) {
                if (i.check_point(event->pos()))
                {
                    is_scaling = true;
                    scaling_start_point = event->pos();
                    scaling_figure = &i;
                    break;
                }
            }
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }
    repaint();
}

void Lab1::mouseMoveEvent(QMouseEvent *event)
{
    switch (current_mode) {
    case move:
        if ((event->buttons() & Qt::LeftButton) && is_moving){
           moving_figure->move(event->pos().x()-moving_start_point.x(), event->pos().y()-moving_start_point.y());
           moving_start_point = event->pos();
        }
        break;

    case rotate:
        if ((event->buttons() & Qt::LeftButton) && is_rotating){
            rotating_figure->set_angle(rotate_angle(rotating_start_point, rotating_figure->get_center(), event->pos()));
            rotating_figure->move(0, 0);
        }
        break;

    case scale:
        if ((event->buttons() & Qt::LeftButton) && is_scaling){
            scaling_figure->set_scale(D_b_points(event->pos(), scaling_figure->get_center()) / D_b_points(scaling_start_point, scaling_figure->get_center()));
            scaling_figure->move(0, 0);
        }
        break;

    default:
        break;
    }
    repaint();
}

void Lab1::mouseReleaseEvent(QMouseEvent *event)
{
    switch (current_mode) {
    case move:
        switch (event->button()) {
        case Qt::LeftButton:
            if (is_moving){
               moving_figure->move(event->pos().x()-moving_start_point.x(), event->pos().y()-moving_start_point.y());
               moving_start_point = event->pos();
               is_moving = false;
            }
            break;

        default:
            break;
        }
        break;

    case rotate:
        switch (event->button()) {
        case Qt::LeftButton:
            if (is_rotating){
               rotating_figure->set_angle(rotate_angle(rotating_start_point, rotating_figure->get_center(), event->pos()));
               rotating_figure->move(0, 0);
               is_rotating = false;
            }
            break;

        default:
            break;
        }
        break;

    case scale:
        switch (event->button()) {
        case Qt::LeftButton:
            if (is_scaling){
                scaling_figure->set_scale(D_b_points(event->pos(), scaling_figure->get_center()) / D_b_points(scaling_start_point, scaling_figure->get_center()));
                scaling_figure->move(0, 0);
               is_scaling = false;
            }
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }
    repaint();
}

qreal Lab1::rotate_angle(const QPoint& start, const QPoint& center, const QPoint& current)
{
    qreal angle = qAtan2(current.y() - center.y(), current.x() - center.x()) - qAtan2(start.y() - center.y(), start.x() - center.x());
    return angle;
}

qreal Lab1::D_b_points(const QPoint& p1, const QPoint& p2)
{
    return qSqrt(qPow(p2.x() - p1.x(), 2) + qPow(p2.y() - p1.y(), 2));
}

QVector<QPair<QPoint, QPoint>> Lab1::cyrus_beck(QPair<QPoint, QPoint> line, const some_points &shape) {
    auto d = line.first - line.second;
    QVector<QPoint> normals;
    auto shapePoints = shape.points;
    for (auto i = 0; i < shapePoints.length(); i++) {
        auto s = shapePoints[(i + 1) % shapePoints.length()];
        auto e = shapePoints[i];
        auto x = e.y() - s.y();
        auto y = s.x() - e.x();
        normals.append(QPoint(x, y));
    }
    double tE = 0;
    double tL = 1;
    for (auto i = 0; i < normals.length(); i++) {
        auto dot_prod = normals[i].x()*normals[i].y() + d.x()*d.y();
        if (dot_prod != 0){
            auto diff = shapePoints[i] - line.first;
            auto t = (normals[i].x()*normals[i].y() + diff.x()*diff.y()) / 1.0 * (-1 * dot_prod);
            if (dot_prod < 0) tE = qMax(tE, t);
            else tL = qMin(tL, t);
        } else
        {
            auto a = shapePoints[i];
            auto b = shapePoints[(i + 1) % shapePoints.length()];
            auto c = line.first;
            if ((b.x() - a.x()) * (c.y() - a.y()) - (b.y() - a.y()) * (c.x() - a.x()) < 0) {
                tE = 1;
                tL = -1;
            }
        }
    }

    if (tE > tL)
        return QVector<QPair<QPoint, QPoint>>({line});

    auto p1 = line.first + (line.second - line.first) * tE;
    auto p2 = line.first + (line.second - line.first) * tL;

    if (tE == 0 && tL == 1) {
        return QVector<QPair<QPoint, QPoint>>();
    }
    if (tE == 0){
        return QVector<QPair<QPoint, QPoint>>({qMakePair(p2, line.second)});
    } else if (tL == 1) {
        return QVector<QPair<QPoint, QPoint>>({qMakePair(line.first, p1)});
    } else {
        return QVector<QPair<QPoint, QPoint>>({qMakePair(line.first, p1), qMakePair(p2, line.second)});
    }
}

void Lab1::figure::add_point(const int &x, const int &y)
{
    origin_points.push_back(QPoint(x, y));
    points.push_back(QPoint(x, y));
}

void Lab1::figure::set_collor(const QColor &collor)
{
    this->collor = collor;
}

void Lab1::figure::set_angle(const qreal &angle){
    this->angle = angle;
}

void Lab1::figure::set_scale(const qreal &scale){
    this->scale = scale;
}

const QPoint& Lab1::figure::get_center() const{
    return center;
}

void Lab1::figure::renew_center(){
    qreal sx, sy, sL;
    sx = sy = sL = 0;
    for (qsizetype i = 0; i < origin_points.length() - 1; i++) {
        QPoint &p1 = origin_points[i];
        QPoint p0;
        if (i == 0)
        {
            p0 = origin_points.back();
        }
        else{
            p0 = origin_points[i-1];
        }
        qreal L = qSqrt(qPow(p1.x() - p0.x(), 2) + qPow(p1.y() - p0.y(), 2));
        sx += (p1.x() + p0.x()) / 2.0 * L;
        sy += (p1.y() + p0.y()) / 2.0 * L;
        sL += L;
    }
    center.rx() = sx / sL;
    center.ry() = sy / sL;
}

void Lab1::figure::paint(QPainter &Painter, const QVector<some_points> &triangles) const
{
    Painter.setPen(QPen(collor, 1, Qt::SolidLine, Qt::FlatCap));
    Painter.setBrush(QBrush(collor, Qt::SolidPattern));
    QVector<QPair<QPoint, QPoint>> lines;
    if (points.length()>1)
    {
        for (qsizetype i = 0; i < points.length() - 1; i++)
        {
            lines.push_back(qMakePair(points[i], points[i+1]));
        }
        lines.push_back(qMakePair(points.first(), points.last()));
    }
    if (points.length()>2)
        for (auto &i: triangles) {
            QVector<QPair<QPoint, QPoint>> new_lines;
            for (auto &j: lines) {
                new_lines += cyrus_beck(j, i);
            }
            lines = new_lines;
        }
    for (auto &i: lines) {
        Painter.drawLine(i.first, i.second);
    }
}

void Lab1::figure::move(const int &x, const int &y){
    for (auto &i: origin_points)
    {
        i.rx() += x;
        i.ry() += y;
    }
    renew_center();
    points = origin_points;
    if (scale != 1.0)
    {
        for (auto &i: points)
        {
            i.rx() = (i.x() - center.x()) * scale + center.x();
            i.ry() = (i.y() - center.y()) * scale + center.y();
        }
    }
    if (angle != 0.0)
    {
        qreal sin_temp = qSin(angle);
        qreal cos_temp = qCos(angle);
        for (auto &i: points)
        {
            i.rx() = center.x() + cos_temp * (i.x() - center.x()) - sin_temp * (i.y() - center.y());
            i.ry() = center.y() + sin_temp * (i.x() - center.x()) + cos_temp * (i.y() - center.y());
        }
    }
}

bool Lab1::figure::check_point(const QPoint &point) const
{
    bool result = false;
    for (qsizetype i = 0, j = points.length() - 1; i < points.length(); j = i++){
        if ( ((points[i].y() <= point.y() && point.y() < points[j].y()) || (points[j].y() <= point.y() && point.y() < points[i].y())) &&
             (points[j].y() - points[i].y() != 0) &&
                 (point.x() > ((points[j].x() - points[i].x()) * (point.y() - points[i].y()) / (points[j].y() - points[i].y()) + points[i].x())) )
                result = !result;
    }
    return result;
}

qsizetype Lab1::figure::points_count() const
{
    return points.length();
}

QVector<Lab1::some_points> Lab1::figure::triangulate() const{
    QVector<Lab1::some_points> triangles;

    QVector<QPoint> remaining_points = points;

    auto prev_remaining_points_len = remaining_points.length();

    while (remaining_points.length() > 3)
    {
        for (auto i = 0; i < remaining_points.length(); i++) {
            auto i1 = i == 0 ? remaining_points.length() - 1: i - 1;
            auto i2 = i;
            auto i3 = (i + 1) % remaining_points.length();
            QVector<QPoint> p;
            p.append(remaining_points[i1]);
            p.append(remaining_points[i2]);
            p.append(remaining_points[i3]);
            auto angle = qAtan2(p[2].y() - p[1].y(), p[2].x() - p[1].x()) - qAtan2(p[0].y() - p[1].y(), p[0].x() - p[1].x());
            if (qFabs(angle) >= M_PI) continue;
            Lab1::some_points triangleCandidate;
            triangleCandidate.points = p;
            for (auto l = 0; l < remaining_points.length(); l++) {
                if (l == i1 || l == i2 || l == i3) continue;
                if (triangleCandidate.check_point(remaining_points[l])) continue;
            }
            triangles.append(triangleCandidate);
            remaining_points.remove(i);
            break;
        }
        if (remaining_points.length() == prev_remaining_points_len)
            break;
        else
            prev_remaining_points_len = remaining_points.length();
    }

    Lab1::some_points temp;
    temp.points = {remaining_points[0], remaining_points[1], remaining_points[2]};
    triangles.append(temp);

    return triangles;
}

bool Lab1::some_points::check_point(const QPoint &point) const
{
    bool result = false;
    for (qsizetype i = 0, j = points.length() - 1; i < points.length(); j = i++){
        if ( ((points[i].y() <= point.y() && point.y() < points[j].y()) || (points[j].y() <= point.y() && point.y() < points[i].y())) &&
             (points[j].y() - points[i].y() != 0) &&
                 (point.x() > ((points[j].x() - points[i].x()) * (point.y() - points[i].y()) / (points[j].y() - points[i].y()) + points[i].x())) )
                result = !result;
    }
    return result;
}

void Lab1::on_pushButton_clicked()
{
    current_mode = add;
    repaint();
}


void Lab1::on_pushButton_2_clicked()
{
    current_mode = move;
    repaint();
}


void Lab1::on_pushButton_3_clicked()
{
    current_mode = rotate;
    repaint();
}


void Lab1::on_pushButton_4_clicked()
{
    current_mode = scale;
    repaint();
}

