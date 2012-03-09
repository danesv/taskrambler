#include "http/parser.h"

#include "utils/http.h"

void
httpParserNewMessage(
		HttpParser   this,
		const char * line,
		const char * lend)
{
	const char * part1, * part2, * part3;
	size_t       len1, len2, len3;

	part1 = line;
	part2 = memchr(line, ' ', lend - line);

	if (NULL == part2) return;

	len1 = part2 - part1;
	for (; *part2 == ' ' && *part2 != 0; part2++);

	part3 = memchr(part2, ' ', lend - part2);

	if (NULL == part3) return;

	len2 = part3 - part2;
	for (; *part3 == ' ' && *part3 != 0; part3++);

	len3 = lend - part3;

	this->current = httpGetMessage(part1, len1, part2, len2, part3, len3);
}

// vim: set ts=4 sw=4:
