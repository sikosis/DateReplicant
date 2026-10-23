#include <Application.h>
#include <Button.h>
#include <Deskbar.h>
#include <Errors.h>
#include <File.h>
#include <FindDirectory.h>
#include <Font.h>
#include <InterfaceDefs.h>
#include <LayoutBuilder.h>
#include <ListView.h>
#include <Message.h>
#include <Path.h>
#include <ScrollView.h>
#include <String.h>
#include <StringItem.h>
#include <StringView.h>
#include <TabView.h>
#include <TextView.h>
#include <View.h>

#include <cstring>
#include <limits>

#include "AppMessages.h"
#include "DateTextView.h"
#include "SettingsWindow.h"

extern const char *settings_file;

enum {
	kMsgReloadSettings = 'dRld',
	kMsgSaveSettings = 'dSav'
};

static const char *help_text =
	"Settings file guide\n"
	"\n"
	"Keep the settings in the displayed order, with one setting on each line.\n"
	"Text values must remain inside double quotes.\n"
	"Save writes the file without refreshing the displays. Apply saves and\n"
	"refreshes the open view and both Deskbar replicants.\n"
	"\n"
	"Background_AM / Background_PM\n"
	"    Path to the image used before or after midday. An empty path uses the\n"
	"    bundled background.\n"
	"\n"
	"FontDay / FontMonth / FontYear\n"
	"    \"font family\" \"font style\" size red green blue alpha \"On|Off\"\n"
	"    Colour channels range from 0 to 255. The final value controls font\n"
	"    antialiasing.\n"
	"\n"
	"PositionDay / PositionMonth / PositionYear\n"
	"    Horizontal and vertical pixel positions, written as: x y\n"
	"\n"
	"DayVisible / MonthVisible / YearVisible\n"
	"    Use \"On\" to show the field or \"Off\" to hide it.\n"
	"\n"
	"DayString / MonthString / YearString\n"
	"    Text formatted with the strftime codes below. Ordinary text is shown\n"
	"    unchanged, and %% displays a percent sign.\n"
	"\n"
	"TextString\n"
	"    Format for the compact text item in Deskbar. The default is\n"
	"    \"%a %d %b %Y\".\n"
	"\n"
	"Date and time format codes\n"
	"\n"
	"%a   Abbreviated weekday name (Fri)\n"
	"%A   Full weekday name (Friday)\n"
	"%b   Abbreviated month name (Oct)\n"
	"%B   Full month name (October)\n"
	"%c   Locale-preferred date and time\n"
	"%C   Century as two digits\n"
	"%d   Day of the month, zero-padded (01–31)\n"
	"%D   Date as %m/%d/%y\n"
	"%e   Day of the month, space-padded ( 1–31)\n"
	"%F   ISO date as %Y-%m-%d\n"
	"%g   Last two digits of the ISO week-based year\n"
	"%G   Full ISO week-based year\n"
	"%h   Same as %b\n"
	"%H   Hour using a 24-hour clock (00–23)\n"
	"%I   Hour using a 12-hour clock (01–12)\n"
	"%j   Day of the year (001–366)\n"
	"%m   Month number (01–12)\n"
	"%M   Minute (00–59)\n"
	"%n   Newline\n"
	"%o   DateReplicant ordinal suffix (st, nd, rd or th)\n"
	"%p   Locale AM or PM marker\n"
	"%r   Locale 12-hour time\n"
	"%R   Time as %H:%M\n"
	"%S   Second (00–60)\n"
	"%t   Tab\n"
	"%T   Time as %H:%M:%S\n"
	"%u   Weekday number, Monday is 1 (1–7)\n"
	"%U   Week number, Sunday first (00–53)\n"
	"%V   ISO week number (01–53)\n"
	"%w   Weekday number, Sunday is 0 (0–6)\n"
	"%W   Week number, Monday first (00–53)\n"
	"%x   Locale-preferred date\n"
	"%X   Locale-preferred time\n"
	"%y   Year without century (00–99)\n"
	"%Y   Full year\n"
	"%z   Numeric time-zone offset\n"
	"%Z   Time-zone name\n"
	"%E?  Locale alternative form of the following date or time code\n"
	"%O?  Locale alternative digits for the following numeric code\n"
	"%%   A literal percent sign\n";

static status_t settingsPath(BPath &path) {
	status_t status = find_directory(B_USER_SETTINGS_DIRECTORY, &path);
	if (status != B_OK)
		return status;

	return path.Append(settings_file);
}
//---------------------------------------------------------------------------------------------------------------------------------//


static BView *createHelpView() {
	BListView *help = new BListView("Help", B_SINGLE_SELECTION_LIST);
	help->SetFont(be_fixed_font);

	BScrollView *scroll = new BScrollView("format legend scroll", help,
		B_FRAME_EVENTS, true, true, B_FANCY_BORDER);
	const char *line_start = help_text;
	while (*line_start != '\0') {
		const char *line_end = strchr(line_start, '\n');
		int32 length = line_end == NULL
			? static_cast<int32>(strlen(line_start))
			: static_cast<int32>(line_end - line_start);
		BString line;
		line.SetTo(line_start, length);
		help->AddItem(new BStringItem(line.String()));
		if (line_end == NULL)
			break;
		line_start = line_end + 1;
	}
	BView *view = new BView("Help", B_WILL_DRAW);
	view->SetViewUIColor(B_PANEL_BACKGROUND_COLOR);
	BLayoutBuilder::Group<>(view, B_VERTICAL, 0.0f)
		.SetInsets(10.0f)
		.Add(scroll);
	return view;
}
//---------------------------------------------------------------------------------------------------------------------------------//


SettingsWindow::SettingsWindow():
	BWindow(BRect(0, 0, 720, 600), "DateReplicant Settings",
		B_TITLED_WINDOW, B_NOT_ZOOMABLE | B_CLOSE_ON_ESCAPE) {
	editor = new BTextView("settings editor");
	editor->SetFontAndColor(be_fixed_font);
	editor->SetWordWrap(false);
	editor->SetDoesUndo(true);

	BScrollView *editor_scroll = new BScrollView("settings scroll", editor,
		B_FRAME_EVENTS, true, true, B_FANCY_BORDER);
	BView *editor_view = new BView("Settings File", B_WILL_DRAW);
	editor_view->SetViewUIColor(B_PANEL_BACKGROUND_COLOR);
	BLayoutBuilder::Group<>(editor_view, B_VERTICAL, 8.0f)
		.SetInsets(10.0f)
		.Add(new BStringView("editor description",
			"Edit Date_settings directly. Keep variable names and quoted values intact."))
		.Add(editor_scroll);

	BTabView *tabs = new BTabView("settings tabs", B_WIDTH_FROM_LABEL);
	tabs->AddTab(editor_view);
	tabs->AddTab(createHelpView());

	status_view = new BStringView("status", "Loading settings" B_UTF8_ELLIPSIS);
	BButton *reload_button = new BButton("reload", "Reload",
		new BMessage(kMsgReloadSettings));
	BButton *save_button = new BButton("save", "Save",
		new BMessage(kMsgSaveSettings));
	BButton *apply_button = new BButton("apply", "Apply",
		new BMessage(kMsgApplySettings));
	apply_button->MakeDefault(true);
	BButton *close_button = new BButton("close", "Close",
		new BMessage(B_QUIT_REQUESTED));

	BLayoutBuilder::Group<>(this, B_VERTICAL, 10.0f)
		.SetInsets(12.0f)
		.Add(tabs, 1.0f)
		.AddGroup(B_HORIZONTAL, 8.0f)
			.Add(status_view, 1.0f)
			.Add(reload_button)
			.Add(save_button)
			.Add(apply_button)
			.Add(close_button)
		.End();

	CenterOnScreen();
	loadSettings();
}
//---------------------------------------------------------------------------------------------------------------------------------//


void SettingsWindow::MessageReceived(BMessage *message) {
	switch (message->what) {
		case kMsgReloadSettings:
			loadSettings();
			break;
		case kMsgSaveSettings:
			saveSettings();
			break;
		case kMsgApplySettings:
			applySettings();
			break;
		default:
			BWindow::MessageReceived(message);
			break;
	}
}
//---------------------------------------------------------------------------------------------------------------------------------//


void SettingsWindow::loadSettings() {
	BPath path;
	status_t status = settingsPath(path);
	if (status != B_OK) {
		setStatus("Unable to locate the settings file", status);
		return;
	}

	BFile file(path.Path(), B_READ_ONLY);
	status = file.InitCheck();
	if (status != B_OK) {
		setStatus("Unable to open the settings file", status);
		return;
	}

	off_t size = 0;
	status = file.GetSize(&size);
	if (status != B_OK
		|| size > static_cast<off_t>(std::numeric_limits<int32>::max())) {
		setStatus("Unable to read the settings file",
			status == B_OK ? B_FILE_TOO_LARGE : status);
		return;
	}

	char *contents = new char[static_cast<size_t>(size) + 1];
	ssize_t bytes_read = file.Read(contents, static_cast<size_t>(size));
	if (bytes_read < 0) {
		delete[] contents;
		setStatus("Unable to read the settings file", bytes_read);
		return;
	}

	contents[bytes_read] = '\0';
	editor->SetText(contents, static_cast<int32>(bytes_read));
	delete[] contents;
	setStatus("Loaded");
}
//---------------------------------------------------------------------------------------------------------------------------------//


void SettingsWindow::applySettings() {
	status_t status = saveSettings();
	if (status != B_OK)
		return;

	status = reloadTextReplicant();
	if (status != B_OK) {
		setStatus("Settings saved, but the Deskbar text could not be refreshed", status);
		return;
	}

	be_app->PostMessage(kMsgApplySettings);
	status_view->SetText("Saved and applied to the open view and Deskbar replicants.");
}
//---------------------------------------------------------------------------------------------------------------------------------//


status_t SettingsWindow::reloadTextReplicant() {
	BDeskbar deskbar;
	if (!deskbar.IsRunning() || !deskbar.HasItem(DateTextView::ItemName()))
		return B_OK;

	status_t status = deskbar.RemoveItem(DateTextView::ItemName());
	if (status != B_OK)
		return status;

	DateTextView text_view;
	return deskbar.AddItem(&text_view);
}
//---------------------------------------------------------------------------------------------------------------------------------//


status_t SettingsWindow::saveSettings() {
	BPath path;
	status_t status = settingsPath(path);
	if (status != B_OK) {
		setStatus("Unable to locate the settings file", status);
		return status;
	}

	BFile file(path.Path(), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
	status = file.InitCheck();
	if (status != B_OK) {
		setStatus("Unable to open the settings file", status);
		return status;
	}

	ssize_t length = editor->TextLength();
	ssize_t written = file.Write(editor->Text(), length);
	if (written != length) {
		status = written < 0 ? written : B_IO_ERROR;
		setStatus("Unable to save the complete settings file", status);
		return status;
	}

	status = file.Sync();
	if (status != B_OK) {
		setStatus("Unable to finish saving the settings file", status);
		return status;
	}

	status_view->SetText("Saved. Choose Apply to refresh the running views.");
	return B_OK;
}
//---------------------------------------------------------------------------------------------------------------------------------//


void SettingsWindow::setStatus(const char *prefix, status_t status) {
	BString text(prefix);
	if (status != B_OK)
		text << ": " << strerror(status);
	status_view->SetText(text.String());
}
//---------------------------------------------------------------------------------------------------------------------------------//
