#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>

#include "dataset.h"

#include "datasettablemodel.h"
#include "enginesync.h"
#include "analyses.h"
#include "widgets/progresswidget.h"

#include "analysisforms/analysisform.h"
#include "asyncloader.h"
#include "optionsform.h"
#include "activitylog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	void open(QString filename);
	~MainWindow();

protected:
	virtual void resizeEvent(QResizeEvent *event) OVERRIDE;
	virtual void dragEnterEvent(QDragEnterEvent *event) OVERRIDE;
	virtual void dropEvent(QDropEvent *event) OVERRIDE;
	virtual void closeEvent(QCloseEvent *event) OVERRIDE;

private:
	Ui::MainWindow *ui;

	AnalysisForm *_currentOptionsWidget;
	DataSetPackage *_package;
	DataSetTableModel *_tableModel;
	Analysis *_currentAnalysis;

	Analyses *_analyses;
	EngineSync* _engineSync;

	void packageChanged(DataSetPackage *package);

	bool closeRequestCheck(bool &isSaving);

	AsyncLoader _loader;
	ProgressWidget *_alert;

	bool _inited;
	bool _isClosed = false;
	bool _dataSetClosing = false;

	AnalysisForm* loadForm(Analysis *analysis);
	void showForm(Analysis *analysis);
	void closeCurrentOptionsWidget();

	QWidget *_buttonPanel;
	QVBoxLayout *_buttonPanelLayout;
	QPushButton *_okButton;
	QPushButton *_runButton;
	QPushButton *_menuButton;

	OptionsForm *_optionsForm;

	std::map<std::string, AnalysisForm *> _analysisForms;

	int _tableViewWidthBeforeOptionsMadeVisible;

	bool _resultsViewLoaded = false;
	bool _openedUsingArgs = false;
	QString _openOnLoadFilename;
	QSettings _settings;
	ActivityLog *_log;
	QString _fatalError;

signals:
	void analysisSelected(int id);
	void analysisUnselected();
	void analysisChangedDownstream(int id, QString options);
	void pushToClipboard(QString mimeType, QString data);

private slots:

	void analysisResultsChangedHandler(Analysis* analysis);
	void analysisSelectedHandler(int id);
	void analysisUnselectedHandler();
	void pushToClipboardHandler(const QString &mimeType, const QString &data);
	void analysisChangedDownstreamHandler(int id, QString options);

	void tabChanged(int index);
	void helpToggled(bool on);
	void dataSetSelected(const QString &filename);
	void dataSetCloseRequested();
	void dataSetLoaded(const QString &dataSetName, DataSetPackage *package, const QString &filename);
	void dataSetLoadFailed(const QString &message);
	void saveFailed(const QString &message);
	void itemSelected(const QString &item);
	void exportSelected(const QString &filename);
	void saveSelected(const QString &filename);
	void saveComplete(const QString &name);

	void adjustOptionsPanelWidth();
	void splitterMovedHandler(int, int);

	void hideOptionsPanel();
	void showOptionsPanel();
	void showTableView();
	void hideTableView();

	void analysisOKed();
	void analysisRunned();
	void analysisRemoved();

	void updateMenuEnabledDisabledStatus();
	void updateUIFromOptions();

	void resultsPageLoaded(bool success);

	void saveKeysSelected();
	void openKeysSelected();

	void illegalOptionStateChanged();
	void fatalError();

	void helpFirstLoaded(bool ok);
	void requestHelpPage(const QString &pageName);
};

#endif // MAINWIDGET_H
