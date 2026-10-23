#ifndef DATETEXTVIEW_H
#define DATETEXTVIEW_H

#include <String.h>
#include <View.h>

class DateTextView: public BView {
	public:
		DateTextView();
		DateTextView(BMessage *archive);

		status_t Archive(BMessage *archive, bool deep = true) const;
		static BArchivable *Instantiate(BMessage *archive);
		static const char *ItemName();

		void AttachedToWindow();
		void Draw(BRect);
		void MouseDown(BPoint where);
		void Pulse();

	private:
		void loadSettings();
		void updateText();

		BString date_format;
		BString text;
};

#endif /* DATETEXTVIEW_H */
