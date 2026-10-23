#include <Application.h>
#include "DatePreferences.h"

const char *app_sig = "application/x-vnd.Be-SimpleDate";
const char *settings_file = "Date_settings";
const char *resources_name = "from OBOS_UI_v2_2 image";

class DateApp: public BApplication {
	public:
		DateApp();
};

DateApp::DateApp(): BApplication(app_sig) {
	DatePreferences *window = new DatePreferences();
	if (window != NULL)
		window->Show();
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

