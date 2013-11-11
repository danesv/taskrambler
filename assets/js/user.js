function User(userSelector, menu)
{
	this.userElement = $(userSelector);
	this.menu        = menu;

	this.update();
}

User.prototype.update = function() {
	$.getJSON("/currentuser/", $.proxy(this._setData, this));
}

User.prototype.clear = function() {
	this.username  = "";
	this.email     = "";
	this.firstname = "";
	this.surname   = "";

	this._displayUser();
}

User.prototype.isEmpty = function() {
	if ("" == this.username) {
		return true;
	}

	return false;
}


User.prototype._setData = function(data) {
	this.username  = data.username;
	this.email     = data.email;
	this.firstname = data.firstname;
	this.surname   = data.surname;

	this._displayUser();
	this.menu.update();
}

User.prototype._displayUser = function() {
	if (this.isEmpty()) {
		this.userElement.empty().append("not logged in");
	} else {
		if ("" == this.firstname || "" == this.surname) {
			this.userElement.empty().append(this.username);
		} else {
			this.userElement.empty()
				.append(this.firstname + " " + this.surname);
		}
	}
}

// vim: set ts=4 sw=4:
