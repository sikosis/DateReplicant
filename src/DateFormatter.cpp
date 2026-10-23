#include "DateFormatter.h"

#include <string>

static const char *ordinalSuffix(int day) {
	int last_two_digits = day % 100;
	if (last_two_digits >= 11 && last_two_digits <= 13)
		return "th";

	switch (day % 10) {
		case 1:
			return "st";
		case 2:
			return "nd";
		case 3:
			return "rd";
		default:
			return "th";
	}
}
//---------------------------------------------------------------------------------------------------------------------------------//


size_t formatDate(char *buffer, size_t bufferSize, const char *format,
	const struct tm *localTime) {
	if (buffer == NULL || bufferSize == 0 || format == NULL || localTime == NULL)
		return 0;

	std::string expanded_format;
	for (const char *current = format; *current != '\0'; current++) {
		if (*current != '%' || current[1] == '\0') {
			expanded_format.append(current, 1);
			continue;
		}

		if (current[1] == '%') {
			expanded_format.append("%%");
			current++;
			continue;
		}

		if (current[1] == 'o') {
			expanded_format.append(ordinalSuffix(localTime->tm_mday));
			current++;
			continue;
		}

		expanded_format.append(current, 2);
		current++;
	}

	return strftime(buffer, bufferSize, expanded_format.c_str(), localTime);
}
//---------------------------------------------------------------------------------------------------------------------------------//
