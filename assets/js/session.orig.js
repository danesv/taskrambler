function Session(sInfo, sId, sUser)
{
	this.eUser    = $(sUser);
	this.eId      = $(sId);
	this.canvas   = $(sInfo + " canvas").get(0);
	this.context  = this.canvas.getContext("2d");

	this.id       = "none"
	this.timeout  = 0;
	this.timeleft = 0;
	this.user     = null;

	//this.draw();
}

Session.prototype.isAthenticated = function(data)
{
}

Session.prototype.authenticate = function(data)
{
	this.username  = data.username;
	this.email     = data.email;
	this.firstname = data.firstname;
	this.surname   = data.surname;

	name = "";
	if ('' == this.username) {
		name = "not logged in";
		$("li.signup").removeClass("hide");
		$("li.login").removeClass("hide");
		$("li.logout").addClass("hide");
	} else {
		if ('' == this.firstname || '' == this.surname) {
			name += this.username;
		} else {
			name += this.firstname + " " + this.surname;
		}
		$("li.signup").addClass("hide");
		$("li.login").addClass("hide");
		$("li.logout").removeClass("hide");
	}

	this.eUser.empty().append(name);
}

Session.prototype.loadJSON = function(data)
{
	// this.stop();

	this.id        = ("0" == data.id)? "none" : data.id;
	this.timeout   = data.timeout * 10;
	this.timeleft  = data.timeleft * 10;

	this.eId.empty().append(this.id);

	this.draw();
	if (0 < this.timeleft)
		this.start();
}

Session.prototype.draw = function()
{
	this.context.fillStyle = "rgb(255, 0, 0)";
	this.context.fillRect(0, 0, this.canvas.width, this.canvas.height);

	this.context.fillStyle = "rgb(0, 255, 0)";
	this.context.fillRect(0, 0,
			this.canvas.width / this.timeout * this.timeleft,
			this.canvas.height);
}

Session.prototype.start = function()
{
	if (null === this.interval) {
		this.interval = setInterval($.proxy(this.process, this), 1000);
	}
}

Session.prototype.process = function()
{
	if (0 >= this.timeleft) {
		this.stop();
	}

	else {
		this.timeleft -= 10;
		this.draw();
	}
}

Session.prototype.stop = function()
{
	clearInterval(this.interval);
	this.interval = null;
	this.id       = "none";
	this.timeout  = 0;
	this.timeleft = 0;
	this.username = "";
	this.email     = "";
	this.firstname = "";
	this.surname   = "";

	this.eId.empty().append("");
	this.eUser.empty().append("not logged in");

	this.draw();
}

// vim: set ts=4 sw=4:
