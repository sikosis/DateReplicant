#include <ClassInfo.h>
#include <Font.h>
#include <InterfaceDefs.h>
#include <Message.h>
#include <Window.h>

#include <cstdio>
#include <cstring>
#include <ctime>

#include "DateTextView.h"
#include "ReplicantMenu.h"

extern const char *app_sig;

static const float horizontal_padding = 4.0;
static const char *date_format = "%a %d %b %Y";
static const char *view_name = "DateReplicant text";

DateTextView::DateTextView():
	BView(BRect(0, 0, 99, 15), view_name, B_FOLLOW_NONE,
		B_WILL_DRAW | B_PULSE_NEEDED) {
}
//---------------------------------------------------------------------------------------------------------------------------------//


DateTextView::DateTextView(BMessage *archive):
	BView(archive) {
}
//---------------------------------------------------------------------------------------------------------------------------------//


status_t DateTextView::Archive(BMessage *archive, bool deep) const {
	status_t status = BView::Archive(archive, deep);
	if (status != B_OK)
		return status;

	return archive->AddString("add_on", app_sig);
}
//---------------------------------------------------------------------------------------------------------------------------------//


BArchivable *DateTextView::Instantiate(BMessage *archive) {
	if (!validate_instantiation(archive, "DateTextView"))
		return NULL;

	return new DateTextView(archive);
}
//---------------------------------------------------------------------------------------------------------------------------------//


const char *DateTextView::ItemName() {
	return view_name;
}
//---------------------------------------------------------------------------------------------------------------------------------//


void DateTextView::AttachedToWindow() {
	BView::AttachedToWindow();
	SetFont(be_plain_font);
	SetViewUIColor(B_PANEL_BACKGROUND_COLOR);
	SetLowUIColor(B_PANEL_BACKGROUND_COLOR);
	updateText();
}
//---------------------------------------------------------------------------------------------------------------------------------//


void DateTextView::Draw(BRect) {
	font_height metrics;
	GetFontHeight(&metrics);

	float text_height = metrics.ascent + metrics.descent;
	float baseline = (Bounds().Height() - text_height) / 2.0 + metrics.ascent;

	SetHighUIColor(B_PANEL_TEXT_COLOR);
	DrawString(text.String(), BPoint(horizontal_padding, baseline));
}
//---------------------------------------------------------------------------------------------------------------------------------//


void DateTextView::MouseDown(BPoint where) {
	int32 buttons = 0;
	if (Window() != NULL && Window()->CurrentMessage() != NULL)
		Window()->CurrentMessage()->FindInt32("buttons", &buttons);

	if ((buttons & B_SECONDARY_MOUSE_BUTTON) != 0) {
		showReplicantMenu(this, where);
		return;
	}

	BView::MouseDown(where);
}
//---------------------------------------------------------------------------------------------------------------------------------//


void DateTextView::Pulse() {
	updateText();
}
//---------------------------------------------------------------------------------------------------------------------------------//


void DateTextView::updateText() {
	time_t current = time(NULL);
	struct tm local_time;
	char buffer[64];

	if (localtime_r(&current, &local_time) == NULL)
		return;

	if (strftime(buffer, sizeof(buffer), date_format, &local_time) == 0)
		return;

	if (strcmp(buffer, text.String()) == 0)
		return;

	text = buffer;
	ResizeTo(StringWidth(text.String()) + horizontal_padding * 2.0,
		Bounds().Height());
	Invalidate();
}
//---------------------------------------------------------------------------------------------------------------------------------//
