#include <Application.h>
#include <Alert.h>
#include <Deskbar.h>

#include <cstring>

#include "DatePreferences.h"
#include "DateTextView.h"

const char *app_sig = "application/x-vnd.Be-SimpleDate";
const char *settings_file = "Date_settings";
const char *resources_name = "from OBOS_UI_v2_2 image";

class DateApp: public BApplication {
	public:
		DateApp();

	private:
		void installTextView();
};

DateApp::DateApp(): BApplication(app_sig) {
	DatePreferences *window = new DatePreferences();
	if (window != NULL)
		window->Show();

	installTextView();
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

