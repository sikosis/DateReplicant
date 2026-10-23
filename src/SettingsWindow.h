#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <Window.h>

class BStringView;
class BTextView;

class SettingsWindow: public BWindow {
	public:
		SettingsWindow();

		void MessageReceived(BMessage *message);

	private:
		void applySettings();
		void loadSettings();
		status_t reloadTextReplicant();
		status_t saveSettings();
		void setStatus(const char *prefix, status_t status = B_OK);

		BTextView *editor;
		BStringView *status_view;
};

#endif /* SETTINGSWINDOW_H */
