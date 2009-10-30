function PainterTest(parent) {
    QWidget.call(this, parent);
}

PainterTest.prototype = new QWidget();

PainterTest.prototype.paintEvent = function() {
    var path = new QPainterPath();
    path.addRect(20, 20, 60, 60);
    path.moveTo(0, 0);
    path.cubicTo(99, 0,  50, 50,  99, 99);
    path.cubicTo(0, 99,  50, 50,  0, 0);

    print("path.elementAt(0): " + path.elementAt(0));

    print("path.elementAt(0).isLineTo(): " + path.elementAt(0).isLineTo());
    print("path.elementAt(0).isCurveTo(): " + path.elementAt(0).isCurveTo());
    print("path.elementAt(0).isMoveTo(): " + path.elementAt(0).isMoveTo());

    print("path.elementAt(1).isLineTo(): " + path.elementAt(1).isLineTo());
    print("path.elementAt(1).isCurveTo(): " + path.elementAt(1).isCurveTo());
    print("path.elementAt(1).isMoveTo(): " + path.elementAt(1).isMoveTo());

    // This shous that if QPainterPath::Element actually had any public
    // constructors we could create an instance
    //Qt.Debug.trace = Qt.Debug.MethodMatches;
    //var foo = new QPainterPath.Element();

    var painter = new QPainter(this);
    painter.fillRect(0, 0, 100, 100, Qt.white);
    painter.setPen(new QPen(new QBrush(new QColor(79, 106, 25)), 1, Qt.SolidLine,
                        Qt.FlatCap, Qt.MiterJoin));
    painter.setBrush(new QBrush(new QColor(122, 163, 39)));

    painter.drawPath(path);
    painter.end();
}

Qt.Debug.trace = Qt.Debug.Calls;
var widget = new PainterTest();
widget.show();