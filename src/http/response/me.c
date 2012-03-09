/**
 * \file
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright (C) 2012  Georg Hopp
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>

#include "class.h"
#include "interface/class.h"

#include "http/response.h"
#include "http/message.h"
#include "http/header.h"

#include "utils/memory.h"


#define RESP_DATA "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n" \
	"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\"\n" \
	" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n" \
	"<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">" \
	"<head>" \
		"<title>My own little Web-App</title>" \
		"<style type=\"text/css\">" \
			"div#randval {" \
				"left: 200px;" \
				"top: 100px;" \
				"position: fixed;" \
				"background-color: white;" \
				"border: 1px solid black;" \
			"}" \
			"div.hide#randval {" \
				"top: -500px;" \
			"}" \
			".small {" \
				"font-size: small;" \
			"}" \
		"</style>" \
		"<script type=\"text/javascript\" src=\"/jquery/\"></script>" \
		"<script>" \
			"$(document).ready(function() {" \
				"var intervalId;" \
				"var vnext = 0;" \
				"var clickclose = function() {" \
					"clearInterval(intervalId);" \
					"vnext = 0;" \
					"$(\"#randval\").addClass(\"hide\");" \
				"};" \
				"var counter = function() {" \
					"if (0 >= vnext) {" \
						"$.getJSON(\"/randval/\", function(data, xhr) {" \
							"var date = new Date(data.ctime * 1000);" \
							"$(\"#ctime\").empty().append(date.toString());" \
							"vnext = data.vnext;" \
							"$(\"#value\").empty().append(data.value);" \
							"$(\"#vnext\").empty().append(vnext);" \
							"$(\"#randval\").on(\"click\", clickclose);" \
						"}).error(function(event, request, settings) {" \
							"clearInterval(intervalId);" \
							"$.get(\"/login/\", function(data) {" \
								"$(\"#randval\")" \
									".off(\"click\", clickclose)" \
									".empty().append(data);" \
							"});" \
						"});" \
						"if ($(\"#randval\").hasClass(\"hide\")) {" \
							"$(\"#randval\").removeClass(\"hide\");" \
						"}" \
					"} else {" \
						"vnext--;" \
						"$(\"#vnext\").empty().append(vnext);" \
					"}" \
				"};" \
				"$(\"#msg\").ajaxError(function(event, request, settings) {" \
					"$(this).append(" \
						"\"<li>Error requesting page \" + " \
						"settings.url + " \
						"\"</li>\");" \
					"vnext = 0;" \
				"});" \
				"$(\"a\").click(function() {" \
					"intervalId = setInterval(counter, 1000);" \
				"});" \
			"});" \
		"</script>" \
	"</head>" \
	"<body>" \
		"<div id=\"randval\" class=\"hide\">" \
			"<span class=\"small\">" \
				"Value created at: <br /><span id=\"ctime\"></span><br>" \
				"Next value in: <span id=\"vnext\"></span><br />" \
			"</span>" \
			"Value: <span id=\"value\"></span>" \
		"</div>" \
		"<div id=\"main\">" \
			"<h1>Testpage</h1>" \
			"Welcome %s<br />" \
			"<img src=\"/image/\" />" \
			"<br /><a href=\"#\">Link</a>" \
		"</div>" \
		"<hr /><div id=\"msg\"></div>" \
	"</body>" \
	"</html>"

HttpResponse
httpResponseMe(char * uname)
{
	HttpResponse response;
	HttpMessage  message;

	response = new(HttpResponse, "HTTP/1.1", 200, "OK");
	message  = (HttpMessage)response;

	httpHeaderAdd(&(message->header),
			new(HttpHeader, CSTRA("Content-Type"), CSTRA("text/html")));
	httpHeaderAdd(&(message->header),
			new(HttpHeader, CSTRA("Set-Cookie"), CSTRA("name=Georg+Hopp")));
	httpHeaderAdd(&(message->header),
			new(HttpHeader, CSTRA("Set-Cookie"), CSTRA("profession=coder")));

	message->type  = HTTP_MESSAGE_BUFFERED;
	message->nbody = sizeof(RESP_DATA)-1-2+strlen(uname); //!< the two are the %s
	message->body  = malloc(message->nbody+1);
	sprintf(message->body, RESP_DATA, uname);
	//memcpy(message->body, RESP_DATA, sizeof(RESP_DATA)-1);

	return response;
}

// vim: set ts=4 sw=4:
