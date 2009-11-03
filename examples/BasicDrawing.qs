/****************************************************************************
**
** Copyright (C) 2008 Trolltech ASA. All rights reserved.
**
** This file is part of the Qt Script Generator project on Trolltech Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

function tr(s) { return s; }



function RenderArea(parent)
{
    QWidget.call(this, parent);

    this.shape = new QVariant(RenderArea.Polygon);
    this.antialiased = false;
    this.transformed = false;
    this.pen = new QPen();
    this.brush = new QBrush();
    this.pixmap = new QPixmap("images/qt-logo.png");

    this.setBackgroundRole(QPalette.Base);
    this.autoFillBackground = true;
}

RenderArea.prototype = new QWidget();

RenderArea.Line = 0;
RenderArea.Points = 1;
RenderArea.Polyline = 2;
RenderArea.Polygon = 3;
RenderArea.Rect = 4;
RenderArea.RoundedRect = 5;
RenderArea.Ellipse = 6;
RenderArea.Arc = 7;
RenderArea.Chord = 8;
RenderArea.Pie = 9;
RenderArea.Path = 10;
RenderArea.Text = 11;
RenderArea.Pixmap = 12;

RenderArea.prototype.getMinimumSizeHint = function()
{
    return new QSize(100, 100);
}

RenderArea.prototype.getSizeHint = function()
{
    return new QSize(400, 200);
}

RenderArea.prototype.setShape = function(shape)
{
    this.shape = shape;
    this.update();
}

RenderArea.prototype.setPen = function(pen)
{
    this.pen = pen;
    this.update();
}

RenderArea.prototype.setBrush = function(brush)
{
    this.brush = brush;
    this.update();
}

RenderArea.prototype.setAntialiased = function(antialiased)
{
    this.antialiased = antialiased;
    this.update();
}

RenderArea.prototype.setTransformed = function(transformed)
{
    this.transformed = transformed;
    this.update();
}

RenderArea.prototype.paintEvent = function(/* event */)
{
    if (RenderArea.points == undefined) {
        RenderArea.points = new QPolygon([  new QPoint(10, 80),
                                            new QPoint(20, 10),
                                            new QPoint(80, 30),
                                            new QPoint(90, 70) ] );
    }

    var rect = new QRect(10, 20, 80, 60);

    var path = new QPainterPath();
    path.moveTo(20, 80);
    path.lineTo(20, 30);
    path.cubicTo(80, 0, 50, 50, 80, 80);

    var startAngle = 20 * 16;
    var arcLength = 120 * 16;

    var painter = new QPainter();
    painter.begin(this);
    painter.setPen(this.pen);
    painter.setBrush(this.brush);
    if (this.antialiased) {
        painter.setRenderHint(QPainter.Antialiasing, true);
        painter.translate(+0.5, +0.5);
    }

    for (var x = 0; x < this.width; x += 100) {
        for (var y = 0; y < this.height; y += 100) {
            painter.save();
            painter.translate(x, y);
            if (this.transformed) {
                painter.translate(50, 50);
                painter.rotate(60.0);
                painter.scale(0.6, 0.9);
                painter.translate(-50, -50);
            }
            
            switch (this.shape.toInt()) {
            case RenderArea.Line:
                painter.drawLine(rect.bottomLeft(), rect.topRight());
                break;
            case RenderArea.Points:
                painter.drawPoints(RenderArea.points);
                break;
            case RenderArea.Polyline:
                painter.drawPolyline(RenderArea.points);
                break;
            case RenderArea.Polygon:
                painter.drawPolygon(RenderArea.points);
                break;
            case RenderArea.Rect:
                painter.drawRect(rect);
                break;
            case RenderArea.RoundedRect:
                painter.drawRoundedRect(rect, 25, 25, Qt.RelativeSize);
                break;
            case RenderArea.Ellipse:
                painter.drawEllipse(rect);
                break;
            case RenderArea.Arc:
                painter.drawArc(rect, startAngle, arcLength);
                break;
            case RenderArea.Chord:
                painter.drawChord(rect, startAngle, arcLength);
                break;
            case RenderArea.Pie:
                painter.drawPie(rect, startAngle, arcLength);
                break;
            case RenderArea.Path:
                painter.drawPath(path);
                break;
            case RenderArea.Text:
                painter.drawText(rect, Qt.AlignCenter, tr("Qt by\nTrolltech"));
                break;
            case RenderArea.Pixmap:
                painter.drawPixmap(10, 10, this.pixmap);
            }
            painter.restore();
        }
    }
    painter.setPen(this.palette.dark().color());
    painter.setBrush(Qt.NoBrush);
    painter.drawRect(0, 0, this.width - 1, this.height - 1);
    painter.end();
}



function Window(parent)
{
    QWidget.call(this, parent);

    this.renderArea = new RenderArea();

    this.shapeComboBox = new QComboBox();
    this.shapeComboBox.addItem(tr("Polygon"), new QVariant(RenderArea.Polygon));
    this.shapeComboBox.addItem(tr("Rectangle"), new QVariant(RenderArea.Rect));
    this.shapeComboBox.addItem(tr("Rounded Rectangle"), new QVariant(RenderArea.RoundedRect));
    this.shapeComboBox.addItem(tr("Ellipse"), new QVariant(RenderArea.Ellipse));
    this.shapeComboBox.addItem(tr("Pie"), new QVariant(RenderArea.Pie));
    this.shapeComboBox.addItem(tr("Chord"), new QVariant(RenderArea.Chord));
    this.shapeComboBox.addItem(tr("Path"), new QVariant(RenderArea.Path));
    this.shapeComboBox.addItem(tr("Line"), new QVariant(RenderArea.Line));
    this.shapeComboBox.addItem(tr("Polyline"), new QVariant(RenderArea.Polyline));
    this.shapeComboBox.addItem(tr("Arc"), new QVariant(RenderArea.Arc));
    this.shapeComboBox.addItem(tr("Points"), new QVariant(RenderArea.Points));
    this.shapeComboBox.addItem(tr("Text"), new QVariant(RenderArea.Text));
    this.shapeComboBox.addItem(tr("Pixmap"), new QVariant(RenderArea.Pixmap));

    this.shapeLabel = new QLabel(tr("&Shape:"));
    this.shapeLabel.setBuddy(this.shapeComboBox);

    this.penWidthSpinBox = new QSpinBox();
    this.penWidthSpinBox.setRange(0, 20);
    this.penWidthSpinBox.specialValueText = tr("0 (cosmetic pen)");

    this.penWidthLabel = new QLabel(tr("Pen &Width:"));
    this.penWidthLabel.setBuddy(this.penWidthSpinBox);

    this.penStyleComboBox = new QComboBox();
    this.penStyleComboBox.addItem(tr("Solid"), new QVariant(Qt.SolidLine));
    this.penStyleComboBox.addItem(tr("Dash"), new QVariant(Qt.DashLine));
    this.penStyleComboBox.addItem(tr("Dot"), new QVariant(Qt.DotLine));
    this.penStyleComboBox.addItem(tr("Dash Dot"), new QVariant(Qt.DashDotLine));
    this.penStyleComboBox.addItem(tr("Dash Dot Dot"), new QVariant(Qt.DashDotDotLine));
    this.penStyleComboBox.addItem(tr("None"), new QVariant(Qt.NoPen));

    this.penStyleLabel = new QLabel(tr("&Pen Style:"));
    this.penStyleLabel.setBuddy(this.penStyleComboBox);

    this.penCapComboBox = new QComboBox();
    this.penCapComboBox.addItem(tr("Flat"), new QVariant(Qt.FlatCap));
    this.penCapComboBox.addItem(tr("Square"), new QVariant(Qt.SquareCap));
    this.penCapComboBox.addItem(tr("Round"), new QVariant(Qt.RoundCap));

    this.penCapLabel = new QLabel(tr("Pen &Cap:"));
    this.penCapLabel.setBuddy(this.penCapComboBox);

    this.penJoinComboBox = new QComboBox();
    this.penJoinComboBox.addItem(tr("Miter"), new QVariant(Qt.MiterJoin));
    this.penJoinComboBox.addItem(tr("Bevel"), new QVariant(Qt.BevelJoin));
    this.penJoinComboBox.addItem(tr("Round"), new QVariant(Qt.RoundJoin));

    this.penJoinLabel = new QLabel(tr("Pen &Join:"));
    this.penJoinLabel.setBuddy(this.penJoinComboBox);

    this.brushStyleComboBox = new QComboBox();
    this.brushStyleComboBox.addItem(tr("Linear Gradient"),
            new QVariant(Qt.LinearGradientPattern));
    this.brushStyleComboBox.addItem(tr("Radial Gradient"),
            new QVariant(Qt.RadialGradientPattern));
    this.brushStyleComboBox.addItem(tr("Conical Gradient"),
            new QVariant(Qt.ConicalGradientPattern));
    this.brushStyleComboBox.addItem(tr("Texture"), new QVariant(Qt.TexturePattern));
    this.brushStyleComboBox.addItem(tr("Solid"), new QVariant(Qt.SolidPattern));
    this.brushStyleComboBox.addItem(tr("Horizontal"), new QVariant(Qt.HorPattern));
    this.brushStyleComboBox.addItem(tr("Vertical"), new QVariant(Qt.VerPattern));
    this.brushStyleComboBox.addItem(tr("Cross"), new QVariant(Qt.CrossPattern));
    this.brushStyleComboBox.addItem(tr("Backward Diagonal"), new QVariant(Qt.BDiagPattern));
    this.brushStyleComboBox.addItem(tr("Forward Diagonal"), new QVariant(Qt.FDiagPattern));
    this.brushStyleComboBox.addItem(tr("Diagonal Cross"), new QVariant(Qt.DiagCrossPattern));
    this.brushStyleComboBox.addItem(tr("Dense 1"), new QVariant(Qt.Dense1Pattern));
    this.brushStyleComboBox.addItem(tr("Dense 2"), new QVariant(Qt.Dense2Pattern));
    this.brushStyleComboBox.addItem(tr("Dense 3"), new QVariant(Qt.Dense3Pattern));
    this.brushStyleComboBox.addItem(tr("Dense 4"), new QVariant(Qt.Dense4Pattern));
    this.brushStyleComboBox.addItem(tr("Dense 5"), new QVariant(Qt.Dense5Pattern));
    this.brushStyleComboBox.addItem(tr("Dense 6"), new QVariant(Qt.Dense6Pattern));
    this.brushStyleComboBox.addItem(tr("Dense 7"), new QVariant(Qt.Dense7Pattern));
    this.brushStyleComboBox.addItem(tr("None"), new QVariant(Qt.NoBrush));

    this.brushStyleLabel = new QLabel(tr("&Brush Style:"));
    this.brushStyleLabel.setBuddy(this.brushStyleComboBox);

    this.otherOptionsLabel = new QLabel(tr("Other Options:"));
    this.antialiasingCheckBox = new QCheckBox(tr("&Antialiasing"));
    this.transformationsCheckBox = new QCheckBox(tr("&Transformations"));

    this.shapeComboBox["activated(int)"].connect(
            this, this.shapeChanged);
    this.penWidthSpinBox["valueChanged(int)"].connect(
            this, this.penChanged);
    this.penStyleComboBox["activated(int)"].connect(
            this, this.penChanged);
    this.penCapComboBox["activated(int)"].connect(
            this, this.penChanged);
    this.penJoinComboBox["activated(int)"].connect(
            this, this.penChanged);
    this.brushStyleComboBox["activated(int)"].connect(
            this, this.brushChanged);
    this.antialiasingCheckBox["toggled(bool)"].connect(
            this.renderArea, this.renderArea.setAntialiased);
    this.transformationsCheckBox["toggled(bool)"].connect(
            this.renderArea, this.renderArea.setTransformed);

    var mainLayout = new QGridLayout();
    mainLayout.setColumnStretch(0, 1);
    mainLayout.setColumnStretch(3, 1);
    mainLayout.addWidget(this.renderArea, 0, 0, 1, 4);
    mainLayout.setRowMinimumHeight(1, 6);
    mainLayout.addWidget(this.shapeLabel, 2, 1, Qt.AlignRight);
    mainLayout.addWidget(this.shapeComboBox, 2, 2);
    mainLayout.addWidget(this.penWidthLabel, 3, 1, Qt.AlignRight);
    mainLayout.addWidget(this.penWidthSpinBox, 3, 2);
    mainLayout.addWidget(this.penStyleLabel, 4, 1, Qt.AlignRight);
    mainLayout.addWidget(this.penStyleComboBox, 4, 2);
    mainLayout.addWidget(this.penCapLabel, 5, 1, Qt.AlignRight);
    mainLayout.addWidget(this.penCapComboBox, 5, 2);
    mainLayout.addWidget(this.penJoinLabel, 6, 1, Qt.AlignRight);
    mainLayout.addWidget(this.penJoinComboBox, 6, 2);
    mainLayout.addWidget(this.brushStyleLabel, 7, 1, Qt.AlignRight);
    mainLayout.addWidget(this.brushStyleComboBox, 7, 2);
    mainLayout.setRowMinimumHeight(8, 6);
    mainLayout.addWidget(this.otherOptionsLabel, 9, 1, Qt.AlignRight);
    mainLayout.addWidget(this.antialiasingCheckBox, 9, 2);
    mainLayout.addWidget(this.transformationsCheckBox, 10, 2);
    this.setLayout(mainLayout);

    this.shapeChanged();
    this.penChanged();
    this.brushChanged();
    this.antialiasingCheckBox.checked = true;

    this.windowTitle = tr("Basic Drawing");
}

Window.prototype = new QWidget();

Window.prototype.shapeChanged = function()
{
    var shape = this.shapeComboBox.itemData(this.shapeComboBox.currentIndex);
    this.renderArea.setShape(shape);
}

Window.prototype.penChanged = function()
{
    var width = this.penWidthSpinBox.value;
    var style = this.penStyleComboBox.itemData(
            this.penStyleComboBox.currentIndex);
    var cap = this.penCapComboBox.itemData(
            this.penCapComboBox.currentIndex);
    var join = this.penJoinComboBox.itemData(
            this.penJoinComboBox.currentIndex);

    this.renderArea.setPen(new QPen(new QBrush(new QColor(Qt.blue)), width, style.toUInt(), cap.toUInt(), join.toUInt()));
}

Window.prototype.brushChanged = function()
{
    var style = new Qt.Enum(this.brushStyleComboBox.itemData(
              this.brushStyleComboBox.currentIndex), "Qt::BrushStyle");

    if (style == Qt.LinearGradientPattern) {
        var linearGradient = new QLinearGradient(0, 0, 100, 100);
        // ### working around issue with Qt.GlobalColor
        linearGradient.setColorAt(0.0, new QColor(Qt.white));
        linearGradient.setColorAt(0.2, new QColor(Qt.green));
        linearGradient.setColorAt(1.0, new QColor(Qt.black));
        this.renderArea.setBrush(new QBrush(linearGradient));
    } else if (style == Qt.RadialGradientPattern) {
        var radialGradient = new QRadialGradient(50, 50, 50, 70, 70);
        radialGradient.setColorAt(0.0, new QColor(Qt.white));
        radialGradient.setColorAt(0.2, new QColor(Qt.green));
        radialGradient.setColorAt(1.0, new QColor(Qt.black));
        this.renderArea.setBrush(new QBrush(radialGradient));
    } else if (style == Qt.ConicalGradientPattern) {
        var conicalGradient = new QConicalGradient(50, 50, 150);
        conicalGradient.setColorAt(0.0, new QColor(Qt.white));
        conicalGradient.setColorAt(0.2, new QColor(Qt.green));
        conicalGradient.setColorAt(1.0, new QColor(Qt.black));
        this.renderArea.setBrush(new QBrush(conicalGradient));
    } else if (style == Qt.TexturePattern) {
        this.renderArea.setBrush(new QBrush(new QPixmap("images/brick.png")));
    } else {
        this.renderArea.setBrush(new QBrush(new QColor(Qt.green), new QPixmap(style)));
    }
}

var win = new Window();
win.show();
// QCoreApplication.exec();
