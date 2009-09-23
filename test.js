include("QLabel");
include("QDialog");
include("QHBoxLayout");
include("QWidget");

l = new QLabel("hello");
l.show();

/*
layout = new QHBoxLayout();
layout.addWidget( new QLabel("hello") );
layout.addWidget( new QLabel("world") );
dialog = new QWidget();
dialog.setLayout( layout );
dialog.setMaximumWidth( 400 );
dialog.setMinimumWidth( 300 );
dialog.show();
*/