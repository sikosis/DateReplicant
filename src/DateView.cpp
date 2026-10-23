#include <Window.h>
#include <Dragger.h>
#include <Message.h>
#include <File.h>
#include <TranslationUtils.h>
#include <BitmapStream.h>
#include <TranslatorRoster.h>
#include <Entry.h>
#include <Roster.h>
#include <Resources.h>
#include <ClassInfo.h>
#include <Alert.h>
#include <cstdio>
#include <cstring>
#include "AppMessages.h"
#include "DateFormatter.h"
#include "DateView.h"
#include "ReplicantMenu.h"

extern const char *app_sig;
extern const char *resources_name;

DateView::DateView(void): BView(BRect(0, 0, 10, 10), "Date", 
	B_NOT_RESIZABLE, B_WILL_DRAW | B_PULSE_NEEDED | B_DRAW_ON_CHILDREN) {
	path_image[1] = new BString();
	path_image[0] = new BString();
	Background = b_image[1] = b_image[0] = NULL;
	
	antialiasing_view[2] = antialiasing_view[1] = 
		antialiasing_view[0] = B_DISABLE_ANTIALIASING;
	
	s_view[2] = s_view[1] = s_view[0] = NULL;
	
	OutString[2] = OutString[1] = OutString[0] = "";
	
	BRect r = Bounds();
	
	r.OffsetTo(B_ORIGIN);
	r.top = r.bottom - 7;
	r.left = r.right - 7;
	
	BDragger *dragger = new BDragger(r, this, B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	
	if (dragger)
		AddChild(dragger);
	
	s_view[0] = new BStringView(BRect(15, 10, 40, 40), "s_view01", "00", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	s_view[1] = new BStringView(BRect(10, 35, 40, 40), "s_view02", "00", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	s_view[2] = new BStringView(BRect(30, 35, 40, 40), "s_view03", "00", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	
	if (s_view[0])
		AddChild(s_view[0]);
		
	if (s_view[1])
		AddChild(s_view[1]);
		
	if (s_view[2])
		AddChild(s_view[2]);
	
	b_image[0] = BTranslationUtils::GetBitmap('JPEG', resources_name);
}
//---------------------------------------------------------------------------------------------------------------------------------//


DateView::DateView(BMessage *archive): BView(archive) {
	path_image[1] = new BString("");
	path_image[0] = new BString("");
	Background = b_image[1] = b_image[0] = NULL;
	s_view[2] = s_view[1] = s_view[0] = NULL;
	std::string s = "";
	antialiasing_view[2] = antialiasing_view[1] = 
		antialiasing_view[0] = B_DISABLE_ANTIALIASING;
		
	OutString[2] = OutString[1] = OutString[0] = "";
	
	archive->FindString("DateView:path_image_0", path_image[0]);	
	archive->FindString("DateView:path_image_1", path_image[1]);
	archive->FindString("DateView:DayString", &OutString[0]);
	archive->FindString("DateView:MonthString", &OutString[1]);
	archive->FindString("DateView:YearString", &OutString[2]);
	
	bool a_1 = false, a_2 = false, a_3 = false;
	
	archive->FindBool("DateView:ANTIALIASING_1", &a_1);
	archive->FindBool("DateView:ANTIALIASING_2", &a_2);
	archive->FindBool("DateView:ANTIALIASING_3", &a_3);

	antialiasing_view[0] = a_1 ? B_FORCE_ANTIALIASING: B_DISABLE_ANTIALIASING;
	antialiasing_view[1] = a_2 ? B_FORCE_ANTIALIASING: B_DISABLE_ANTIALIASING;
	antialiasing_view[2] = a_3 ? B_FORCE_ANTIALIASING: B_DISABLE_ANTIALIASING;

	for (int i = 0; i < CountChildren(); i++) {
		if (is_kind_of(ChildAt(i), BStringView)) {
			s = (cast_as(ChildAt(i), BStringView))->Name();
			
			if (s == "s_view01")
				s_view[0] = cast_as(ChildAt(i), BStringView);
			else
			
			if (s == "s_view02")
				s_view[1] = cast_as(ChildAt(i), BStringView);
			else			
			
			if (s == "s_view03")
				s_view[2] = cast_as(ChildAt(i), BStringView);
		}
	}

	entry_ref ref;
	be_roster->FindApp(app_sig, &ref);

	BFile file(&ref, B_READ_ONLY);
	
	BResources r(&file);
	size_t size = 0;
	
	const void *data = r.LoadResource('JPEG', resources_name, &size);
        
    if (data) {
    	BMemoryIO stream(data, size);
    	
        b_image[0] = BTranslationUtils::GetBitmap(&stream);
        
        if (b_image[0])
        	Background = b_image[0];
	}
}
//---------------------------------------------------------------------------------------------------------------------------------//


DateView::~DateView() {
	if (b_image[0])
		delete b_image[0];
		
	if (b_image[1])
		delete b_image[1];
		
	if (path_image[0])
		delete path_image[0];

	if (path_image[1])
		delete path_image[1];
}
//---------------------------------------------------------------------------------------------------------------------------------//


status_t DateView::Archive(BMessage *archive, bool deep) const {
	archive->AddString("add_on", app_sig);
	
	if (path_image[0])
		archive->AddString("DateView:path_image_0", path_image[0]->String());
		
	if (path_image[1])
		archive->AddString("DateView:path_image_1", path_image[1]->String());
		
	archive->AddString("DateView:DayString", OutString[0].String());
	archive->AddString("DateView:MonthString", OutString[1].String());
	archive->AddString("DateView:YearString", OutString[2].String());
	
	archive->AddBool("DateView:ANTIALIASING_1",
		(antialiasing_view[0] == B_FORCE_ANTIALIASING)? true: false);
	archive->AddBool("DateView:ANTIALIASING_2",
		(antialiasing_view[1] == B_FORCE_ANTIALIASING)? true: false);
	archive->AddBool("DateView:ANTIALIASING_3",
		(antialiasing_view[2] == B_FORCE_ANTIALIASING)? true: false);

	return BView::Archive(archive, deep);
}
//---------------------------------------------------------------------------------------------------------------------------------//


BArchivable *DateView::Instantiate(BMessage *archive) {
	if ( !validate_instantiation(archive, "DateView") )
		return NULL;
		
	return new DateView(archive);
}
//---------------------------------------------------------------------------------------------------------------------------------//


void DateView::applyChanges(void) {
	if (Background) {
		ResizeTo(Background->Bounds().Width(), Background->Bounds().Height());
		SetViewBitmap(Background, B_FOLLOW_H_CENTER);
	}

	for (unsigned i = 0; i < sizeof(s_view)/sizeof(s_view[0]); i++)
		if (s_view[i]) {
			s_view[i]->SetViewColor(B_TRANSPARENT_32_BIT);
			s_view[i]->ResizeToPreferred();
		}
}
//---------------------------------------------------------------------------------------------------------------------------------//


void DateView::AttachedToWindow(void) {
	BView::AttachedToWindow();
	applySettings();
	Pulse();
	Invalidate();
}
//---------------------------------------------------------------------------------------------------------------------------------//


void DateView::MouseDown(BPoint where) {
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


void DateView::Pulse(void) {
	time_t current = time(0);
	struct tm local_time;
	
	if (localtime_r(&current, &local_time) == NULL)
		return;

	for (unsigned i = 0; i < sizeof(s_view)/sizeof(s_view[0]); i++) {
		if (s_view[i]) {
			if (formatDate(tmp_buf, sizeof(tmp_buf), OutString[i].String(),
					&local_time) > 0 && strcmp(tmp_buf, s_view[i]->Text()) != 0)
				s_view[i]->SetText(tmp_buf);
		}
	}

	BBitmap *background = local_time.tm_hour >= 12 ? b_image[1] : b_image[0];
	if (background == NULL)
		background = local_time.tm_hour >= 12 ? b_image[0] : b_image[1];

	if (background != Background) {
		Background = background;
		applyChanges();
		Invalidate();
	}
}
//---------------------------------------------------------------------------------------------------------------------------------//


BBitmap *DateView::LoadImage(const char *file_name) {
	BFile file(file_name, B_READ_ONLY);
	BTranslatorRoster *roster = BTranslatorRoster::Default();
	BBitmapStream stream;
	BBitmap *result = NULL;
	
	if (file.InitCheck() == B_OK) {
		if (roster->Translate(&file, NULL, NULL, &stream,
	  		B_TRANSLATOR_BITMAP) < B_OK)
			return NULL;
	}
	else {
			std::string m ="Bad image file\n\""+ std::string(file_name)+"\"";
			(new BAlert("Simple Date", m.c_str(),"OK"))->Go();
			return NULL;	
		}
		
	stream.DetachBitmap(&result);
	return result;
}
//---------------------------------------------------------------------------------------------------------------------------------//


void DateView::applySettings(bool changes) {
	for (unsigned i = 0; i < sizeof(b_image)/sizeof(b_image[0]); i++) {
		if (path_image[i]->Length() == 0)
			continue;

		BBitmap *image = LoadImage(path_image[i]->String());
		if (image != NULL) {
			delete b_image[i];
			b_image[i] = image;
		}
	}
	
	if (b_image[0])
		Background = b_image[0];

	BFont font;
	
	for (unsigned i = 0; i < sizeof(s_view)/sizeof(s_view[0]); i++)
		if (s_view[i]) {
			s_view[i]->SetViewColor(B_TRANSPARENT_32_BIT);
			s_view[i]->GetFont(&font);
			font.SetFlags(antialiasing_view[i]);
			s_view[i]->SetFont(&font);
		}
	
	if (changes)	
		applyChanges();
}
//---------------------------------------------------------------------------------------------------------------------------------//


void DateView::MessageReceived(BMessage *msg) {
	switch(msg->what) {
		case B_ABOUT_REQUESTED:
			sendDateReplicantMessage(kMsgShowAbout);
			break;
		default:
			BView::MessageReceived(msg);
	}
}
//---------------------------------------------------------------------------------------------------------------------------------//

