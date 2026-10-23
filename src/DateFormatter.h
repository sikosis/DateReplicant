#ifndef DATEFORMATTER_H
#define DATEFORMATTER_H

#include <cstddef>
#include <ctime>

size_t formatDate(char *buffer, size_t bufferSize, const char *format,
	const struct tm *localTime);

#endif /* DATEFORMATTER_H */
