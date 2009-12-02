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

function CalendarWidget(parent) { 
    QWidget.call(this, parent);

    this.createPreviewGroupBox();
    this.createGeneralOptionsGroupBox();
    this.createDatesGroupBox();
    this.createTextFormatsGroupBox();
    
    var layout = new QGridLayout();
    layout.addWidget(this.previewGroupBox, 0, 0);
    layout.addWidget(this.generalOptionsGroupBox, 0, 1);
    layout.addWidget(this.datesGroupBox, 1, 0);
    layout.addWidget(this.textFormatsGroupBox, 1, 1);
    layout.sizeConstraint = QLayout.SetFixedSize;
    this.setLayout(layout);

    this.previewLayout.setRowMinimumHeight(0, this.calendar.sizeHint.height());
    this.previewLayout.setColumnMinimumWidth(0, this.calendar.sizeHint.width());
    
    this.setWindowTitle(tr("Calendar Widget"));
}

CalendarWidget.prototype = new QWidget();

CalendarWidget.prototype.localeChanged = function(index) {
    this.calendar.setLocale(this.localeCombo.itemData(index));
}

CalendarWidget.prototype.firstDayChanged = function(index) {
    this.calendar.firstDayOfWeek = this.firstDayCombo.itemData(index);
}

CalendarWidget.prototype.selectionModeChanged = function(index) {
    this.calendar.selectionMode = this.selectionModeCombo.itemData(index);
}

CalendarWidget.prototype.horizontalHeaderChanged = function(index) {
    this.calendar.horizontalHeaderFormat = this.horizontalHeaderCombo.itemData(index);
}

CalendarWidget.prototype.verticalHeaderChanged = function(index) {
    this.calendar.verticalHeaderFormat = this.verticalHeaderCombo.itemData(index);
}

CalendarWidget.prototype.selectedDateChanged = function() {
    this.currentDateEdit.setDate(this.calendar.selectedDate);
}

CalendarWidget.prototype.minimumDateChanged = function(date) {
    this.calendar.minimumDate = date;
    this.maximumDateEdit.setDate(this.calendar.maximumDate);
}

CalendarWidget.prototype.maximumDateChanged = function(date) {
    this.calendar.setMaximumDate(date);
    this.minimumDateEdit.setDate(this.calendar.minimumDate);
}

CalendarWidget.prototype.weekdayFormatChanged = function() {
    var format = new QTextCharFormat();    
    format.setForeground(new QBrush(this.weekdayColorCombo.itemData(this.weekdayColorCombo.currentIndex).valueOf()));

    this.calendar.setWeekdayTextFormat(Qt.DayOfWeek.Monday, format);
    this.calendar.setWeekdayTextFormat(Qt.DayOfWeek.Tuesday, format);
    this.calendar.setWeekdayTextFormat(Qt.DayOfWeek.Wednesday, format);
    this.calendar.setWeekdayTextFormat(Qt.DayOfWeek.Thursday, format);
    this.calendar.setWeekdayTextFormat(Qt.DayOfWeek.Friday, format);
}

CalendarWidget.prototype.weekendFormatChanged = function() {
    var format = new QTextCharFormat();
    format.setForeground(new QBrush(this.weekendColorCombo.itemData(this.weekendColorCombo.currentIndex).valueOf()));
    
    this.calendar.setWeekdayTextFormat(Qt.DayOfWeek.Saturday, format);
    this.calendar.setWeekdayTextFormat(Qt.DayOfWeek.Sunday, format);
}

CalendarWidget.prototype.reformatHeaders = function() {
    var text = this.headerTextFormatCombo.currentText;
    var format = new QTextCharFormat();

    if (text == tr("Bold")) {
        format.setFontWeight(QFont.Bold);
    } else if (text == tr("Italic")) {
        format.setFontItalic(true);
    } else if (text == tr("Green")) {
        format.setForeground(new QBrush(new QColor(Qt.green)));
    }
    
    this.calendar.setHeaderTextFormat(format);
}

CalendarWidget.prototype.reformatCalendarPage = function() {
    var mayFirstFormat = new QTextCharFormat();
    if (this.mayFirstCheckBox.isChecked)
        mayFirstFormat.setForeground(new QBrush(new QColor(Qt.red)));

    var firstFridayFormat = new QTextCharFormat();
    if (this.firstFridayCheckBox.isChecked)
        firstFridayFormat.setForeground(new QBrush(new QColor(Qt.blue)));
    
    var date = new Date(this.calendar.yearShown(), this.calendar.monthShown(), 1); //fixme

    this.calendar.setDateTextFormat(new Date(date.year, 5, 1), mayFirstFormat);

    date.setDate(date.year, date.month, 1);
 //fixme
 //   while (date.dayOfWeek != Qt.Friday)
 //       date = date.addDays(1);
    this.calendar.setDateTextFormat(date, firstFridayFormat);
}

CalendarWidget.prototype.createPreviewGroupBox = function() {
    this.previewGroupBox = new QGroupBox(tr("Preview"));

    this.calendar = new QCalendarWidget();
    this.calendar.minimumDate = new Date(1900, 1, 1);
    this.calendar.maximumDate = new Date(3000, 1, 1);
    this.calendar.setGridVisible(true);

    this.calendar.currentPageChanged.connect(this, this.reformatCalendarPage);

    this.previewLayout = new QGridLayout();
    this.previewLayout.addWidget(this.calendar, 0, 0, Qt.AlignmentFlag.AlignCenter);
    this.previewGroupBox.setLayout(this.previewLayout);
}

CalendarWidget.prototype.createGeneralOptionsGroupBox = function() {
    this.generalOptionsGroupBox = new QGroupBox(tr("General Options"));
    
    this.localeCombo = new QComboBox();
    var curLocaleIndex = -1;
    var index = 0;

    

    for (var lang in QLocale.Language) {
        var countries = QLocale.countriesForLanguage(lang);
            
        for (var country in countries) {
            var label = QLocale.languageToString(lang);
            label += "/";
            label += QLocale.countryToString(country);
            var locale = new QLocale(lang, country);
            if (this.locale.language() == lang && this.locale().country() == country)
                curLocaleIndex = index;

            this.localeCombo.addItem(label, locale);
            ++index;
        }
    }

    if (curLocaleIndex != -1)
        localeCombo.setCurrentIndex(curLocaleIndex);
    this.localeLabel = new QLabel(tr("&Locale"));
    this.localeLabel.setBuddy(this.localeCombo);
    
    this.firstDayCombo = new QComboBox();
    this.firstDayCombo.addItem(tr("Sunday"), new QVariant(Qt.Sunday));
    this.firstDayCombo.addItem(tr("Monday"), new QVariant(Qt.Monday));
    this.firstDayCombo.addItem(tr("Tuesday"), new QVariant(Qt.Tuesday));
    this.firstDayCombo.addItem(tr("Wednesday"), new QVariant(Qt.Wednesday));
    this.firstDayCombo.addItem(tr("Thursday"), new QVariant(Qt.Thursday));
    this.firstDayCombo.addItem(tr("Friday"), new QVariant(Qt.Friday));
    this.firstDayCombo.addItem(tr("Saturday"), new QVariant(Qt.Saturday));
    
    this.firstDayLabel = new QLabel(tr("Wee&k starts on:"));
    this.firstDayLabel.setBuddy(this.firstDayCombo);
    
    this.selectionModeCombo = new QComboBox();
    this.selectionModeCombo.addItem(tr("Single selection"),
                               new QVariant(QCalendarWidget.SingleSelection));
    this.selectionModeCombo.addItem(tr("None"), new QVariant(QCalendarWidget.NoSelection));
    
    this.selectionModeLabel = new QLabel(tr("&Selection mode:"));
    this.selectionModeLabel.setBuddy(this.selectionModeCombo);
    
    this.gridCheckBox = new QCheckBox(tr("&Grid"));
    this.gridCheckBox.setChecked(this.calendar.gridVisible);
    
    this.navigationCheckBox = new QCheckBox(tr("&Navigation bar"));
    this.navigationCheckBox.setChecked(true);
    
    this.horizontalHeaderCombo = new QComboBox();
    this.horizontalHeaderCombo.addItem(tr("Single letter day names"),
                                  new QVariant(QCalendarWidget.SingleLetterDayNames));
    this.horizontalHeaderCombo.addItem(tr("Short day names"),
                                  new QVariant(QCalendarWidget.ShortDayNames));
    this.horizontalHeaderCombo.addItem(tr("None"),
                                  new QVariant(QCalendarWidget.NoHorizontalHeader));
    this.horizontalHeaderCombo.setCurrentIndex(1);
    
    this.horizontalHeaderLabel = new QLabel(tr("&Horizontal header:"));
    this.horizontalHeaderLabel.setBuddy(this.horizontalHeaderCombo);
    
    this.verticalHeaderCombo = new QComboBox();
    this.verticalHeaderCombo.addItem(tr("ISO week numbers"),
                                new QVariant(QCalendarWidget.ISOWeekNumbers));
    this.verticalHeaderCombo.addItem(tr("None"), new QVariant(QCalendarWidget.NoVerticalHeader));
    
    this.verticalHeaderLabel = new QLabel(tr("&Vertical header:"));
    this.verticalHeaderLabel.setBuddy(this.verticalHeaderCombo);
    
    this.localeCombo['currentIndexChanged(int)'].connect(this, this.localeChanged);
    this.firstDayCombo['currentIndexChanged(int)'].connect(this, this.firstDayChanged);
    this.selectionModeCombo['currentIndexChanged(int)'].connect(this, this.selectionModeChanged);
    this.gridCheckBox.toggled.connect(this, this.calendar.setGridVisible);        
    this.navigationCheckBox.toggled.connect(this, this.calendar.setNavigationBarVisible);
    this.horizontalHeaderCombo['currentIndexChanged(int)'].connect(this, this.horizontalHeaderChanged);
    this.verticalHeaderCombo['currentIndexChanged(int)'].connect(this, "verticalHeaderChanged");
    
    var checkBoxLayout = new QHBoxLayout();
    checkBoxLayout.addWidget(this.gridCheckBox, 0, 1);  //FIXME
    checkBoxLayout.addStretch();
    checkBoxLayout.addWidget(this.navigationCheckBox, 0, 1);  //FIXME
    var outerLayout = new QGridLayout();
    outerLayout.addWidget(this.localeLabel, 0, 0);
    outerLayout.addWidget(this.localeCombo, 0, 1);
    outerLayout.addWidget(this.firstDayLabel, 1, 0);
    outerLayout.addWidget(this.firstDayCombo, 1, 1);
    outerLayout.addWidget(this.selectionModeLabel, 2, 0);
    outerLayout.addWidget(this.selectionModeCombo, 2, 1);
    outerLayout.addLayout(checkBoxLayout, 3, 0, 1, 2);
    outerLayout.addWidget(this.horizontalHeaderLabel, 4, 0);
    outerLayout.addWidget(this.horizontalHeaderCombo, 4, 1);
    outerLayout.addWidget(this.verticalHeaderLabel, 5, 0);
    outerLayout.addWidget(this.verticalHeaderCombo, 5, 1);

    this.generalOptionsGroupBox.setLayout(outerLayout);
     
    this.firstDayChanged(this.firstDayCombo.currentIndex);
    this.selectionModeChanged(this.selectionModeCombo.currentIndex);
    this.horizontalHeaderChanged(this.horizontalHeaderCombo.currentIndex);
    this.verticalHeaderChanged(this.verticalHeaderCombo.currentIndex);
}

CalendarWidget.prototype.createDatesGroupBox = function() {
    this.datesGroupBox = new QGroupBox(tr("Dates"));
    
    this.minimumDateEdit = new QDateEdit();
    this.minimumDateEdit.displayFormat = "MMM d, yyyy";
    this.minimumDateEdit.setDateRange(this.calendar.minimumDate,
                                 this.calendar.maximumDate);
    this.minimumDateEdit.setDate(this.calendar.minimumDate);
    

    this.minimumDateLabel = new QLabel(tr("&Minimum Date:"));
    this.minimumDateLabel.setBuddy(this.minimumDateEdit);
    
    this.currentDateEdit = new QDateEdit();
    this.currentDateEdit.displayFormat = "MMM d, yyyy";
    this.currentDateEdit.setDate(this.calendar.selectedDate);
    this.currentDateEdit.setDateRange(this.calendar.minimumDate,
                                 this.calendar.maximumDate);
    
    this.currentDateLabel = new QLabel(tr("&Current Date:"));
    this.currentDateLabel.setBuddy(this.currentDateEdit);
    
    this.maximumDateEdit = new QDateEdit();
    this.maximumDateEdit.displayFormat = "MMM d, yyyy";
    this.maximumDateEdit.setDateRange(this.calendar.minimumDate,
                                 this.calendar.maximumDate);
    this.maximumDateEdit.setDate(this.calendar.maximumDate);

    this.maximumDateLabel = new QLabel(tr("Ma&ximum Date:"));
    this.maximumDateLabel.setBuddy(this.maximumDateEdit);
    
    this.currentDateEdit.dateChanged.connect(this, this.calendar.setSelectedDate);
    this.calendar.selectionChanged.connect(this, this.selectedDateChanged);
    this.minimumDateEdit.dateChanged.connect(this, this.minimumDateChanged);
    this.maximumDateEdit.dateChanged.connect(this, this.maximumDateChanged);
    
    var dateBoxLayout = new QGridLayout();
    dateBoxLayout.addWidget(this.currentDateLabel, 1, 0);
    dateBoxLayout.addWidget(this.currentDateEdit, 1, 1);
    dateBoxLayout.addWidget(this.minimumDateLabel, 0, 0);
    dateBoxLayout.addWidget(this.minimumDateEdit, 0, 1);
    dateBoxLayout.addWidget(this.maximumDateLabel, 2, 0);
    dateBoxLayout.addWidget(this.maximumDateEdit, 2, 1);
    dateBoxLayout.setRowStretch(3, 1);
    
    this.datesGroupBox.setLayout(dateBoxLayout);
}

CalendarWidget.prototype.createTextFormatsGroupBox = function() {
    this.textFormatsGroupBox = new QGroupBox(tr("Text Formats"));
    
    this.weekdayColorCombo = this.createColorComboBox();
    this.weekdayColorCombo.setCurrentIndex(this.weekdayColorCombo.findText(tr("Black")));
    
    this.weekdayColorLabel = new QLabel(tr("&Weekday color:"));
    this.weekdayColorLabel.setBuddy(this.weekdayColorCombo);
    
    this.weekendColorCombo = this.createColorComboBox();
    this.weekendColorCombo.setCurrentIndex(this.weekendColorCombo.findText(tr("Red")));
    
    this.weekendColorLabel = new QLabel(tr("Week&end color:"));
    this.weekendColorLabel.setBuddy(this.weekendColorCombo);
    
    this.headerTextFormatCombo = new QComboBox();
    this.headerTextFormatCombo.addItem(tr("Bold"));
    this.headerTextFormatCombo.addItem(tr("Italic"));
    this.headerTextFormatCombo.addItem(tr("Plain"));
   
    this.headerTextFormatLabel = new QLabel(tr("&Header text:"));
    this.headerTextFormatLabel.setBuddy(this.headerTextFormatCombo);
    
    this.firstFridayCheckBox = new QCheckBox(tr("&First Friday in blue"));
    
    this.mayFirstCheckBox = new QCheckBox(tr("May &1 in red"));
    
    this.weekdayColorCombo['currentIndexChanged(int)'].connect(this, this.weekdayFormatChanged);
    this.weekendColorCombo['currentIndexChanged(int)'].connect(this, this.weekendFormatChanged);
    this.headerTextFormatCombo['currentIndexChanged(int)'].connect(this, this.reformatHeaders);
    this.firstFridayCheckBox.toggled.connect(this, this.reformatCalendarPage);
    this.mayFirstCheckBox.toggled.connect(this, this.reformatCalendarPage);
    
    var checkBoxLayout = new QHBoxLayout();
    checkBoxLayout.addWidget(this.firstFridayCheckBox, 0, 1);
    checkBoxLayout.addStretch();
    checkBoxLayout.addWidget(this.mayFirstCheckBox, 0, 1);
     
    var outerLayout = new QGridLayout();
    outerLayout.addWidget(this.weekdayColorLabel, 0, 0);
    outerLayout.addWidget(this.weekdayColorCombo, 0, 1);
    outerLayout.addWidget(this.weekendColorLabel, 1, 0);
    outerLayout.addWidget(this.weekendColorCombo, 1, 1);
    outerLayout.addWidget(this.headerTextFormatLabel, 2, 0);
    outerLayout.addWidget(this.headerTextFormatCombo, 2, 1);
    outerLayout.addLayout(checkBoxLayout, 3, 0, 1, 2);
    this.textFormatsGroupBox.setLayout(outerLayout);
  
    this.weekdayFormatChanged();
    this.weekendFormatChanged();
    this.reformatHeaders();
    this.reformatCalendarPage();
}

CalendarWidget.prototype.createColorComboBox = function() {
    var comboBox = new QComboBox();
    comboBox.addItem(tr("Red"), QVariant.fromValue(new QColor(Qt.red)));
    comboBox.addItem(tr("Blue"), QVariant.fromValue(new QColor(Qt.blue)));
    comboBox.addItem(tr("Black"), QVariant.fromValue(new QColor(Qt.black)));
    comboBox.addItem(tr("Magenta"), QVariant.fromValue(new QColor(Qt.magenta)));
    return comboBox;
}


var calendarWidget = new CalendarWidget(null);
calendarWidget.show();

QCoreApplication.exec();
