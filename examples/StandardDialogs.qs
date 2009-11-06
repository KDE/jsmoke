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


function Dialog(parent)
{
    QDialog.call(this, parent);

    this.errorMessageDialog = new QErrorMessage(this);

    var frameStyle = QFrame.Sunken | QFrame.Panel;

    this.integerLabel = new QLabel;
    this.integerLabel.setFrameStyle(frameStyle);
    var integerButton =
            new QPushButton(tr("QInputDialog::get&Integer()"));

    this.doubleLabel = new QLabel;
    this.doubleLabel.setFrameStyle(frameStyle);
    var doubleButton =
            new QPushButton(tr("QInputDialog::get&Double()"));

    this.itemLabel = new QLabel;
    this.itemLabel.setFrameStyle(frameStyle);
    var itemButton = new QPushButton(tr("QInputDialog::getIte&m()"));

    this.textLabel = new QLabel;
    this.textLabel.setFrameStyle(frameStyle);
    var textButton = new QPushButton(tr("QInputDialog::get&Text()"));

    this.colorLabel = new QLabel;
    this.colorLabel.setFrameStyle(frameStyle);
    var colorButton = new QPushButton(tr("QColorDialog::get&Color()"));

    this.fontLabel = new QLabel;
    this.fontLabel.setFrameStyle(frameStyle);
    var fontButton = new QPushButton(tr("QFontDialog::get&Font()"));

    this.directoryLabel = new QLabel;
    this.directoryLabel.setFrameStyle(frameStyle);
    var directoryButton =
            new QPushButton(tr("QFileDialog::getE&xistingDirectory()"));

    this.openFilePaths = ".";
    this.openFileNameLabel = new QLabel;
    this.openFileNameLabel.setFrameStyle(frameStyle);
    var openFileNameButton =
            new QPushButton(tr("QFileDialog::get&OpenFileName()"));

    this.openFileNamesLabel = new QLabel;
    this.openFileNamesLabel.setFrameStyle(frameStyle);
    var openFileNamesButton =
            new QPushButton(tr("QFileDialog::&getOpenFileNames()"));

    this.saveFileNameLabel = new QLabel;
    this.saveFileNameLabel.setFrameStyle(frameStyle);
    var saveFileNameButton =
            new QPushButton(tr("QFileDialog::get&SaveFileName()"));

    this.criticalLabel = new QLabel;
    this.criticalLabel.setFrameStyle(frameStyle);
    var criticalButton =
            new QPushButton(tr("QMessageBox::critica&l()"));

    this.informationLabel = new QLabel;
    this.informationLabel.setFrameStyle(frameStyle);
    var informationButton =
            new QPushButton(tr("QMessageBox::i&nformation()"));

    this.questionLabel = new QLabel;
    this.questionLabel.setFrameStyle(frameStyle);
    var questionButton =
            new QPushButton(tr("QMessageBox::&question()"));

    this.warningLabel = new QLabel;
    this.warningLabel.setFrameStyle(frameStyle);
    var warningButton = new QPushButton(tr("QMessageBox::&warning()"));

    this.errorLabel = new QLabel;
    this.errorLabel.setFrameStyle(frameStyle);
    var errorButton =
            new QPushButton(tr("QErrorMessage::show&M&essage()"));

    integerButton["clicked()"].connect(this, this.setInteger);
    doubleButton["clicked()"].connect(this, this.setDouble);
    itemButton["clicked()"].connect(this, this.setItem);
    textButton["clicked()"].connect(this, this.setText);
    colorButton["clicked()"].connect(this, this.setColor);
    fontButton["clicked()"].connect(this, this.setFont);
    directoryButton["clicked()"].connect(
            this, this.setExistingDirectory);
    openFileNameButton["clicked()"].connect(
            this, this.setOpenFileName);
    openFileNamesButton["clicked()"].connect(
            this, this.setOpenFileNames);
    saveFileNameButton["clicked()"].connect(
            this, this.setSaveFileName);
    criticalButton["clicked()"].connect(this, this.criticalMessage);
    informationButton["clicked()"].connect(
            this, this.informationMessage);
    questionButton["clicked()"].connect(this, this.questionMessage);
    warningButton["clicked()"].connect(this, this.warningMessage);
    errorButton["clicked()"].connect(this, this.errorMessage);

    this.useNative = new QCheckBox(this);
    this.useNative.text = tr("Use native file dialog");
    this.useNative.checked = true;

    var layout = new QGridLayout;
    layout.setColumnStretch(1, 1);
    layout.setColumnMinimumWidth(1, 250);
    layout.addWidget(integerButton, 0, 0);
    layout.addWidget(this.integerLabel, 0, 1);
    layout.addWidget(doubleButton, 1, 0);
    layout.addWidget(this.doubleLabel, 1, 1);
    layout.addWidget(itemButton, 2, 0);
    layout.addWidget(this.itemLabel, 2, 1);
    layout.addWidget(textButton, 3, 0);
    layout.addWidget(this.textLabel, 3, 1);
    layout.addWidget(colorButton, 4, 0);
    layout.addWidget(this.colorLabel, 4, 1);
    layout.addWidget(fontButton, 5, 0);
    layout.addWidget(this.fontLabel, 5, 1);
    layout.addWidget(directoryButton, 6, 0);
    layout.addWidget(this.directoryLabel, 6, 1);
    layout.addWidget(openFileNameButton, 7, 0);
    layout.addWidget(this.openFileNameLabel, 7, 1);
    layout.addWidget(openFileNamesButton, 8, 0);
    layout.addWidget(this.openFileNamesLabel, 8, 1);
    layout.addWidget(saveFileNameButton, 9, 0);
    layout.addWidget(this.saveFileNameLabel, 9, 1);
    layout.addWidget(criticalButton, 10, 0);
    layout.addWidget(this.criticalLabel, 10, 1);
    layout.addWidget(informationButton, 11, 0);
    layout.addWidget(this.informationLabel, 11, 1);
    layout.addWidget(questionButton, 12, 0);
    layout.addWidget(this.questionLabel, 12, 1);
    layout.addWidget(warningButton, 13, 0);
    layout.addWidget(this.warningLabel, 13, 1);
    layout.addWidget(errorButton, 14, 0);
    layout.addWidget(this.errorLabel, 14, 1);
    layout.addWidget(this.useNative, 15, 0);
    this.setLayout(layout);

    this.windowTitle = tr("Standard Dialogs");
}

Dialog.MESSAGE = tr("<p>Message boxes have a caption, a text, "
               + "and any number of buttons, each with standard or custom texts."
               + "<p>Click a button to close the message box. Pressing the Esc button "
               + "will activate the detected escape button (if any).");

Dialog.prototype = new QDialog();

Dialog.prototype.setInteger = function()
{
    var i = QInputDialog.getInteger(this, tr("QInputDialog::getInteger()"),
        tr("Percentage:"), 25, 0, 100, 1, 0);
    if (i != null)
        this.integerLabel.text = i;
}

Dialog.prototype.setDouble = function()
{
    var d = QInputDialog.getDouble(this, tr("QInputDialog::getDouble()"),
        tr("Amount:"), 37.56, -10000, 10000, 2, 0);
    if (d != null)
        this.doubleLabel.text = d;
}

Dialog.prototype.setItem = function()
{
    var items = new Array(tr("Spring"), tr("Summer"), tr("Fall"), tr("Winter"));

    var item = QInputDialog.getItem(this, tr("QInputDialog::getItem()"),
        tr("Season:"), items, 0, false, 0);
    if (item != null & item.length != 0)
        this.itemLabel.text = item;
}

Dialog.prototype.setText = function()
{
    var text = QInputDialog.getText(this, tr("QInputDialog::getText()"),
                                         tr("User name:"), QLineEdit.Normal,
        QDir.home().dirName(), 0);
    if (text != null && text.length != 0)
        this.textLabel.text = text;
}

Dialog.prototype.setColor = function()
{
    var color = QColorDialog.getColor(new QColor(Qt.green), this);
    if (color.isValid()) {
        this.colorLabel.text = color.name();
        this.colorLabel.palette = new QPalette(color);
        this.colorLabel.autoFillBackground = true;
    }
}

Dialog.prototype.setFont = function()
{
    var font = QFontDialog.getFont(0, new QFont(this.fontLabel.text), this);
    if (font != null) {
        this.fontLabel.text = font.key();
        this.fontLabel.font = font;
    }
}

Dialog.prototype.setExistingDirectory = function()
{
    var options = QFileDialog.DontResolveSymlinks | QFileDialog.ShowDirsOnly;
    if (!this.useNative.checked)
        options |= QFileDialog.DontUseNativeDialog;
    var directory = QFileDialog.getExistingDirectory(this,
                                tr("QFileDialog::getExistingDirectory()"),
                                this.directoryLabel.text,
                                options);
    if (directory.length != 0)
        this.directoryLabel.text = directory;
}

Dialog.prototype.setOpenFileName = function()
{
    var options = 0;
    if (!this.useNative.checked)
        options |= QFileDialog.DontUseNativeDialog;
    var selectedFilter;
    var fileName = QFileDialog.getOpenFileName(this,
                                tr("QFileDialog::getOpenFileName()"),
                                this.openFileNameLabel.text,
                                tr("All Files (*);;Text Files (*.txt)"),
                                selectedFilter,
                                options);
    if (fileName.length != 0)
        this.openFileNameLabel.text = fileName;
}

Dialog.prototype.setOpenFileNames = function()
{
    var options = 0;
    if (!this.useNative.checked)
        options |= QFileDialog.DontUseNativeDialog;
    var selectedFilter;
    var files = QFileDialog.getOpenFileNames(
                                this, tr("QFileDialog::getOpenFileNames()"),
                                this.openFilesPath,
                                tr("All Files (*);;Text Files (*.txt)"),
                                selectedFilter,
                                options);
    if (files.length != 0) {
        this.openFilesPath = files[0];
        this.openFileNamesLabel.text = "[" + files.join(", ") + "]";
    }
}

Dialog.prototype.setSaveFileName = function()
{
    var options = 0;
    if (!this.useNative.checked)
        options |= QFileDialog.DontUseNativeDialog;
    var selectedFilter;
    var fileName = QFileDialog.getSaveFileName(this,
                                tr("QFileDialog::getSaveFileName()"),
                                this.saveFileNameLabel.text,
                                tr("All Files (*);;Text Files (*.txt)"),
                                selectedFilter,
                                options);
    if (fileName.length != 0)
        this.saveFileNameLabel.text = fileName;
}

Dialog.prototype.criticalMessage = function()
{
    var reply = QMessageBox.critical(this, tr("QMessageBox::critical()"),
        Dialog.MESSAGE, QMessageBox.Abort | QMessageBox.Retry | QMessageBox.Ignore);
    if (reply == QMessageBox.Abort)
        this.criticalLabel.text = tr("Abort");
    else if (reply == QMessageBox.Retry)
        this.criticalLabel.text = tr("Retry");
    else
        this.criticalLabel.text = tr("Ignore");
}

Dialog.prototype.informationMessage = function()
{
    var reply = QMessageBox.information(this, tr("QMessageBox::information()"), Dialog.MESSAGE);
    if (reply == QMessageBox.Ok)
        this.informationLabel.text = tr("OK");
    else
        this.informationLabel.text = tr("Escape");
}

Dialog.prototype.questionMessage = function()
{
    var reply = QMessageBox.question(this, tr("QMessageBox::question()"),
        Dialog.MESSAGE, new Qt.Enum(QMessageBox.Yes | QMessageBox.No | QMessageBox.Cancel, "QMessageBox::StandardButtons"));
    if (reply == QMessageBox.Yes)
        this.questionLabel.text = tr("Yes");
    else if (reply == QMessageBox.No)
        this.questionLabel.text = tr("No");
    else
        this.questionLabel.text = tr("Cancel");
}

Dialog.prototype.warningMessage = function()
{
    var msgBox = new QMessageBox(QMessageBox.Warning, tr("QMessageBox::warning()"),
                       Dialog.MESSAGE, 0, this);
    msgBox.addButton(tr("Save &Again"), QMessageBox.AcceptRole);
    msgBox.addButton(tr("&Continue"), QMessageBox.RejectRole);
    if (msgBox.exec() == QMessageBox.AcceptRole)
        this.warningLabel.text = tr("Save Again");
    else
        this.warningLabel.text = tr("Continue");
}

Dialog.prototype.errorMessage = function()
{
    this.errorMessageDialog.showMessage(
            tr("This dialog shows and remembers error messages. "
               + "If the checkbox is checked (as it is by default), "
               + "the shown message will be shown again, "
               + "but if the user unchecks the box the message "
               + "will not appear again if QErrorMessage::showMessage() "
               + "is called with the same message."));
    this.errorLabel.text = tr("If the box is unchecked, the message "
                           + "won't appear again.");
};

(function() {
    var translatorFileName = "qt_";
    translatorFileName += QLocale.system().name();
    var translator = new QTranslator(qApp);
    if (translator.load(translatorFileName, QLibraryInfo.location(QLibraryInfo.TranslationsPath)))
        qApp.installTranslator(translator);

    var dialog = new Dialog();
    return dialog.exec();
})();
