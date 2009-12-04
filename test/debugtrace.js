/*
    Example script to show how to turn debugging traces on and off
*/

Qt.Debug.trace = Qt.Debug.Properties | Qt.Debug.GC;

var path = QDir.currentPath();
print("path: " + path);

Qt.Debug.trace = Qt.Debug.None;

var date = new Date();

Qt.Debug.trace = Qt.Debug.MethodMatches | Qt.Debug.GC;

var datetime = new QDateTime(date);
print("day: " + datetime.date().day());

Qt.Debug.trace = Qt.Debug.None;

layout = new QHBoxLayout();
layout.addWidget( new QLabel("hello") );
layout.addWidget( new QLabel("world") );

Qt.Debug.trace = Qt.Debug.Virtual | Qt.Debug.Calls;

dialog = new QWidget();
dialog.setLayout( layout );
dialog.setMaximumWidth( 400 );
dialog.setMinimumWidth( 300 );

print("dialog.objectName: " + dialog.objectName);


var size = dialog.size;
print("size width " + size.width());

var hidden = dialog.isHidden();
print("hidden: " + hidden);

dialog.show();

Qt.Debug.trace = Qt.Debug.None;

hidden = dialog.isHidden();
print("hidden: " + hidden);

QCoreApplication.instance().exec();

