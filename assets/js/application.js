function Application()
{
}

Application.prototype.init = function(menu) {
	this.session = new Session("#sessid", "#sessinfo");
	this.user    = new User("#user", menu);

	$(window).focus($.proxy(this.session.update, this.session));

	$.getJSON("/version/", function(data) {
		this.version = data.version;
		$("#version").empty().append("version: " + this.version);
	});
}

Application.prototype.logout = function() {
	$.ajax({
		dataType : "json",
		url      : "/authenticate/",
		type     : "DELETE",
		success  : $.proxy(this.user.update, this.user),
		complete : $.proxy(this.session.update, this.session)
	});
}

Application.prototype.login = function(ev) {
	this._sendForm("#login", "/authenticate/", ev);
}

Application.prototype.signup = function(ev) {
	this._sendForm("#signup", "/signup/", ev);
}


Application.prototype._sendForm = function(id, url, ev) {
	ev.preventDefault();
	$.ajax({
		dataType : "json",
		url      : url,
		type     : "POST",
		data     : $(id + " form").serialize(),
		success  : $.proxy(this._formSuccess, this, id),
		complete : $.proxy(this.session.update, this.session)
	});
}

Application.prototype._formSuccess = function(id) {
	this.user.update();
	$(id + "-container").addClass("hide");
}

// vim: set ts=4 sw=4:
