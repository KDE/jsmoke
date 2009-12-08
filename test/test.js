// Qt.Debug.trace = Qt.Debug.Calls | Qt.Debug.Properties;

/*
var path = QDir.currentPath();
print("path: " + path);

var date = new Date();
var datetime = new QDateTime(date);
print("day: " + datetime.date().day());
*/

layout = new QHBoxLayout();
print("layout: " + layout);
layout.addWidget( new QLabel("hello") );
layout.addWidget( new QLabel("world") );

dialog = new QWidget();
dialog.setLayout( layout );
dialog.setMaximumWidth( 400 );
dialog.setMinimumWidth( 300 );

Qt.Debug.trace = Qt.Debug.Properties;
print("dialog.objectName: " + dialog.objectName);
dialog.objectName = "My Dialog";
print("dialog.objectName: " + dialog.objectName);
Qt.Debug.trace = Qt.Debug.None;


Qt.Debug.trace = Qt.Debug.Properties;
var size = dialog.size;
Qt.Debug.trace = Qt.Debug.None;
print("size: " + size);
// print("size width " + size.width());

var hidden = dialog.isHidden();
print("hidden: " + hidden);

dialog.show();

hidden = dialog.isHidden();
print("hidden: " + hidden);

QCoreApplication.instance().exec();
