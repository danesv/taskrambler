function ServerVal(eId)
{
	this.eId      = eId;
	this.eCtime   = eId + " span:eq(1)";
	this.eVnext   = eId + " span:eq(2)";
	this.eValue   = eId + " span:eq(3)";

	this.interval = null;
	this.ctime    = null;
	this.vnext    = 0;
	this.value    = null;
}

ServerVal.prototype.loadJSON = function(data)
{
	this.ctime = new Date(data.ctime * 1000);
	this.vnext = data.vnext;
	this.value = data.value;

	$.getJSON("/sessinfo/", $.proxy(sess.loadJSON, sess));
	this.show();
}

ServerVal.prototype.show = function()
{
	$(this.eCtime).empty().append(this.ctime.toString());
	$(this.eVnext).empty().append(this.vnext);
	$(this.eValue).empty().append(this.value);

	if ($(this.eId).hasClass("hide")) {
		$(this.eId).removeClass("hide");
	}
}

ServerVal.prototype.start = function()
{
	if (null === this.interval) {
		this.interval = setInterval($.proxy(this.process, this), 1000);
	}
}

ServerVal.prototype.process = function()
{
	if (0 >= this.vnext) {
		$.getJSON("/randval/", $.proxy(this.loadJSON, this))
			.error($.proxy(function(xhr) {
				this.stop();
				$("#msg").append("AJAX error (" + xhr.status + "): ");
				switch(xhr.status) {
					case 403:
						$("#msg").append(
							"Please log in to access this function.<br />");
						break;

					default:
						$("#msg").append(
							"Unhandled - " + xhr.responseText + "<br />");
						break;
				}
			}, this));
	}

	else {
		this.vnext--;
		$(this.eVnext).empty().append(this.vnext);
	}
}

ServerVal.prototype.stop = function()
{
	$(this.eId).addClass("hide");

	clearInterval(this.interval);
	this.interval = null;
	this.vnext    = 0;
}

// vim: set ts=4 sw=4:
