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
		void Pulse();

	private:
		void updateText();

		BString text;
};

#endif /* DATETEXTVIEW_H */
