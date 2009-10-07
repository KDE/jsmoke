include("QLabel");
include("QDialog");
include("QHBoxLayout");
include("QWidget");
include("QVariant");
include("QDir");

var path = QDir.currentPath();
print("path: " + path);

layout = new QHBoxLayout();

d = new Date();
print("ENTER for loop");
for (var name in Date) {
   print("name: " + name);
}
print("EXIT for loop");

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

