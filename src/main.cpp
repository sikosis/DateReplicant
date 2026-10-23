#include <Application.h>
#include <Alert.h>
#include <Deskbar.h>
#include <Message.h>

#include <cstring>

#include "AboutWindow.h"
#include "AppMessages.h"
#include "DatePreferences.h"
#include "DateTextView.h"
#include "SettingsWindow.h"

const char *app_sig = "application/x-vnd.Be-SimpleDate";
const char *settings_file = "Date_settings";
const char *resources_name = "from OBOS_UI_v2_2 image";

class DateApp: public BApplication {
	public:
		DateApp();
		void AboutRequested();
		void MessageReceived(BMessage *message);

	private:
		void applySettings();
		void installTextView();
		void showSettings();
};

DateApp::DateApp(): BApplication(app_sig) {
	DatePreferences *window = new DatePreferences();
	if (window != NULL)
		window->Show();

	installTextView();
}
//---------------------------------------------------------------------------------------------------------------------------------//


void DateApp::AboutRequested() {
	(new AboutWindow())->Show();
}
//---------------------------------------------------------------------------------------------------------------------------------//


void DateApp::MessageReceived(BMessage *message) {
	switch (message->what) {
		case kMsgShowSettings:
			showSettings();
			break;
		case kMsgShowAbout:
		case B_ABOUT_REQUESTED:
			AboutRequested();
			break;
		case kMsgApplySettings:
			applySettings();
			break;
		default:
			BApplication::MessageReceived(message);
			break;
	}
}
//---------------------------------------------------------------------------------------------------------------------------------//


void DateApp::applySettings() {
	for (int32 index = 0; index < CountWindows(); index++) {
		DatePreferences *preferences = dynamic_cast<DatePreferences *>(WindowAt(index));
		if (preferences == NULL)
			continue;

		if (preferences->Lock()) {
			preferences->ReloadSettings();
			preferences->RefreshDeskbarReplicant();
			preferences->Unlock();
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------------//


void DateApp::installTextView() {
	BDeskbar deskbar;
	if (!deskbar.IsRunning() || deskbar.HasItem(DateTextView::ItemName()))
		return;

	DateTextView text_view;
	status_t status = deskbar.AddItem(&text_view);
	if (status != B_OK) {
		BAlert *alert = new BAlert("DateReplicant",
			strerror(status), "OK");
		alert->Go();
	}
}
//---------------------------------------------------------------------------------------------------------------------------------//


void DateApp::showSettings() {
	for (int32 index = 0; index < CountWindows(); index++) {
		SettingsWindow *settings = dynamic_cast<SettingsWindow *>(WindowAt(index));
		if (settings == NULL)
			continue;

		if (settings->Lock()) {
			settings->Activate(true);
			settings->Unlock();
		}
		return;
	}

	(new SettingsWindow())->Show();
}
//---------------------------------------------------------------------------------------------------------------------------------//


int main() {
	try {
		DateApp app;
		app.Run();
	}
	catch (...) {
		return 1;
	}

	return 0;
}
//---------------------------------------------------------------------------------------------------------------------------------//

