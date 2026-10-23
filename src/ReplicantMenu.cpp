#include <Deskbar.h>
#include <InterfaceDefs.h>
#include <MenuItem.h>
#include <Message.h>
#include <Messenger.h>
#include <PopUpMenu.h>
#include <Roster.h>
#include <View.h>

#include "AppMessages.h"
#include "ReplicantMenu.h"

extern const char *app_sig;

void sendDateReplicantMessage(uint32 what) {
	BMessage message(what);
	BMessenger application(app_sig);
	if (application.IsValid() && application.SendMessage(&message) == B_OK)
		return;

	be_roster->Launch(app_sig, &message);
}
//---------------------------------------------------------------------------------------------------------------------------------//


void showReplicantMenu(BView *owner, BPoint where, const char *deskbarItemName) {
	BPopUpMenu *menu = new BPopUpMenu("DateReplicant menu", false, false);
	menu->AddItem(new BMenuItem("Settings" B_UTF8_ELLIPSIS,
		new BMessage(kMsgShowSettings)));
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("About DateReplicant" B_UTF8_ELLIPSIS,
		new BMessage(kMsgShowAbout)));
	if (deskbarItemName != NULL) {
		menu->AddSeparatorItem();
		menu->AddItem(new BMenuItem("Quit", new BMessage(kMsgQuitReplicant)));
	}

	BMenuItem *selected = menu->Go(owner->ConvertToScreen(where), false, true);
	uint32 what = selected != NULL && selected->Message() != NULL
		? selected->Message()->what : 0;

	delete menu;

	if (what == kMsgQuitReplicant && deskbarItemName != NULL) {
		BDeskbar deskbar;
		deskbar.RemoveItem(deskbarItemName);
	} else if (what != 0) {
		sendDateReplicantMessage(what);
	}
}
//---------------------------------------------------------------------------------------------------------------------------------//
