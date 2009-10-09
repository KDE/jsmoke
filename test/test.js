include("QLabel");
include("QDialog");
include("QHBoxLayout");
include("QWidget");
include("QVariant");
include("QDir");
include("QDateTime");

var path = QDir.currentPath();
print("path: " + path);

var date = new Date();
var datetime = new QDateTime(date);
print("day: " + datetime.date().day());

layout = new QHBoxLayout();
print("layout.prototype: " + layout.prototype);
layout.addWidget( new QLabel("hello") );
layout.addWidget( new QLabel("world") );

dialog = new QWidget();
dialog.setLayout( layout );
dialog.setMaximumWidth( 400 );
dialog.setMinimumWidth( 300 );

var size = dialog.size();
print("size width " + size.width());

var hidden = dialog.isHidden();
print("hidden: " + hidden);

dialog.show();

hidden = dialog.isHidden();
print("hidden: " + hidden);

