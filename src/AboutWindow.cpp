#include "AboutWindow.h"

#include <AppFileInfo.h>
#include <Application.h>
#include <Bitmap.h>
#include <Button.h>
#include <File.h>
#include <Font.h>
#include <InterfaceDefs.h>
#include <LayoutBuilder.h>
#include <Message.h>
#include <Mime.h>
#include <Roster.h>
#include <SeparatorView.h>
#include <Size.h>
#include <String.h>
#include <StringView.h>
#include <TabView.h>
#include <View.h>

class IconView: public BView {
	public:
		IconView(BBitmap *icon);
		~IconView();

		void Draw(BRect updateRect);

	private:
		BBitmap *icon;
};

class LinkView: public BStringView {
	public:
		LinkView(const char *name, const char *label, const char *url);

		void AttachedToWindow();
		void MouseDown(BPoint point);

	private:
		BString url;
};

static BBitmap *loadAppIcon() {
	app_info appInfo;
	if (be_app->GetAppInfo(&appInfo) != B_OK)
		return NULL;

	BFile app_file(&appInfo.ref, B_READ_ONLY);
	if (app_file.InitCheck() != B_OK)
		return NULL;

	BBitmap *icon = new BBitmap(BRect(0, 0, 31, 31), B_RGBA32);
	if (icon->InitCheck() != B_OK) {
		delete icon;
		return NULL;
	}

	BAppFileInfo app_file_info(&app_file);
	if (app_file_info.GetIcon(icon, B_LARGE_ICON) != B_OK) {
		delete icon;
		return NULL;
	}

	return icon;
}
//---------------------------------------------------------------------------------------------------------------------------------//


static BString appVersion() {
	BString version("Version 0.1");
	app_info appInfo;
	if (be_app->GetAppInfo(&appInfo) != B_OK)
		return version;

	BFile app_file(&appInfo.ref, B_READ_ONLY);
	BAppFileInfo app_file_info(&app_file);
	version_info versionInfo;
	if (app_file_info.GetVersionInfo(&versionInfo, B_APP_VERSION_KIND) == B_OK) {
		if (versionInfo.minor == 0) {
			version.SetToFormat("Version %lu.%lu",
				(unsigned long)versionInfo.major,
				(unsigned long)versionInfo.middle);
		} else {
			version.SetToFormat("Version %lu.%lu.%lu",
				(unsigned long)versionInfo.major,
				(unsigned long)versionInfo.middle,
				(unsigned long)versionInfo.minor);
		}
	}

	return version;
}
//---------------------------------------------------------------------------------------------------------------------------------//


static BStringView *heading(const char *name, const char *text) {
	BStringView *view = new BStringView(name, text);
	view->SetFont(be_bold_font);
	return view;
}
//---------------------------------------------------------------------------------------------------------------------------------//


static BView *createCreditsView() {
	BView *view = new BView("Credits", B_WILL_DRAW);
	view->SetViewUIColor(B_PANEL_BACKGROUND_COLOR);

	BLayoutBuilder::Group<>(view, B_VERTICAL, 5.0f)
		.SetInsets(12.0f)
		.Add(heading("project heading", "Project"))
		.Add(new BStringView("modernisation", "Modernised and maintained by Sikosis"))
		.AddStrut(8.0f)
		.Add(heading("heritage heading", "Heritage"))
		.Add(new BStringView("original author", "Original Date 0.01 by Vlad B. (_sl@mail.ru)"))
		.Add(new BStringView("background credit",
			"Default background from the OBOS GUI prototype"))
		.AddStrut(8.0f)
		.Add(heading("thanks heading", "Special Thanks"))
		.Add(new BStringView("thanks one", "Sergei Dolgov, Roman Kopko and Ilya Nedoluzhko"))
		.Add(new BStringView("thanks two", "The Haiku project and HaikuArchives community"))
		.AddGlue();

	return view;
}
//---------------------------------------------------------------------------------------------------------------------------------//


static BView *createLicencesView() {
	BView *view = new BView("Licences", B_WILL_DRAW);
	view->SetViewUIColor(B_PANEL_BACKGROUND_COLOR);

	BLayoutBuilder::Group<>(view, B_VERTICAL, 5.0f)
		.SetInsets(12.0f)
		.Add(heading("date licence heading", "DateReplicant"))
		.Add(new BStringView("date licence",
			"The imported Date 0.01 source did not include licence terms."))
		.AddStrut(8.0f)
		.Add(heading("haiku licence heading", "Haiku"))
		.Add(new BStringView("haiku licence", "Haiku APIs are provided under the MIT License."))
		.Add(new LinkView("haiku licence link", "MIT License",
			"https://opensource.org/license/mit"))
		.AddGlue();

	return view;
}
//---------------------------------------------------------------------------------------------------------------------------------//


IconView::IconView(BBitmap *application_icon):
	BView("application icon", B_WILL_DRAW), icon(application_icon) {
	SetViewUIColor(B_PANEL_BACKGROUND_COLOR);
	SetExplicitMinSize(BSize(96, 96));
	SetExplicitMaxSize(BSize(96, 96));
}
//---------------------------------------------------------------------------------------------------------------------------------//


IconView::~IconView() {
	delete icon;
}
//---------------------------------------------------------------------------------------------------------------------------------//


void IconView::Draw(BRect update_rect) {
	(void)update_rect;
	if (icon == NULL)
		return;

	SetDrawingMode(B_OP_ALPHA);
	SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_OVERLAY);
	DrawBitmap(icon, icon->Bounds(), Bounds(), B_FILTER_BITMAP_BILINEAR);
}
//---------------------------------------------------------------------------------------------------------------------------------//


LinkView::LinkView(const char *name, const char *label, const char *link):
	BStringView(name, label), url(link) {
	SetToolTip(link);
	BFont font;
	GetFont(&font);
	font.SetFace(font.Face() | B_UNDERSCORE_FACE);
	SetFont(&font);
}
//---------------------------------------------------------------------------------------------------------------------------------//


void LinkView::AttachedToWindow() {
	BStringView::AttachedToWindow();
	SetViewUIColor(B_PANEL_BACKGROUND_COLOR);

	rgb_color background = ui_color(B_PANEL_BACKGROUND_COLOR);
	int32 luminance = (299 * background.red + 587 * background.green
		+ 114 * background.blue) / 1000;
	if (luminance < 128)
		SetHighColor(105, 190, 255);
	else
		SetHighColor(0, 76, 168);
}
//---------------------------------------------------------------------------------------------------------------------------------//


void LinkView::MouseDown(BPoint point) {
	(void)point;
	const char *arguments[] = {url.String()};
	be_roster->Launch("text/html", 1, arguments);
}
//---------------------------------------------------------------------------------------------------------------------------------//


AboutWindow::AboutWindow():
	BWindow(BRect(0, 0, 640, 620), "About DateReplicant", B_TITLED_WINDOW,
		B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_AUTO_UPDATE_SIZE_LIMITS
			| B_CLOSE_ON_ESCAPE) {
	BStringView *title = new BStringView("title", "DateReplicant");
	BFont title_font(*be_bold_font);
	title_font.SetSize(be_plain_font->Size() * 2.0f);
	title->SetFont(&title_font);

	BString version_text = appVersion();
	BStringView *version = new BStringView("version", version_text.String());
	BStringView *description = new BStringView("description",
		"Graphical and text date replicants for Haiku.");

	BTabView *tabs = new BTabView("about tabs", B_WIDTH_FROM_LABEL);
	tabs->AddTab(createCreditsView());
	tabs->AddTab(createLicencesView());

	BButton *close_button = new BButton("close", "Close",
		new BMessage(B_QUIT_REQUESTED));
	close_button->MakeDefault(true);

	BLayoutBuilder::Group<>(this, B_VERTICAL, 10.0f)
		.SetInsets(18.0f)
		.AddGroup(B_HORIZONTAL, 16.0f)
			.Add(new IconView(loadAppIcon()))
			.AddGroup(B_VERTICAL, 4.0f)
				.Add(title)
				.Add(version)
				.AddStrut(4.0f)
				.Add(description)
				.Add(new LinkView("source link", "github.com/sikosis/DateReplicant",
					"https://github.com/sikosis/DateReplicant"))
				.AddGlue()
			.End()
		.End()
		.Add(new BSeparatorView(B_HORIZONTAL))
		.Add(tabs, 1.0f)
		.AddGroup(B_HORIZONTAL, 0.0f)
			.AddGlue()
			.Add(close_button)
		.End();

	CenterOnScreen();
}
//---------------------------------------------------------------------------------------------------------------------------------//
