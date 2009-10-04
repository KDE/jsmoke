include("QLabel");
include("QDialog");
include("QHBoxLayout");
include("QWidget");

layout = new QHBoxLayout();
layout.addWidget( new QLabel("hello") );
layout.addWidget( new QLabel("world") );
dialog = new QWidget();
dialog.setLayout( layout );
dialog.setMaximumWidth( 400 );
dialog.setMinimumWidth( 300 );
print(dialog);
var size = dialog.size();
print("size width " + size.width());

var hidden = dialog.isHidden();
print("hidden: " + hidden);
//var width = size.width();
// print("width: " + width);
// print(width);
dialog.show();

hidden = dialog.isHidden();
print("hidden: " + hidden);

