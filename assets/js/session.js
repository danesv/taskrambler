function Session(sId)
{
	this.eSid     = $(sId + " span");
	this.canvas   = $(sId + " canvas").get(0);
	this.context  = this.canvas.getContext("2d");

	this.id       = "none"
	this.timeout  = 0;
	this.timeleft = 0;
	this.username = "";
	this.email     = "";
	this.firstname = "";
	this.surname   = "";
	this.interval = null;

	this.draw();
}

Session.prototype.loadJSON = function(data)
{
	this.stop();

	this.id       = ("0" == data.id)? "none" : data.id;
	//this.timeout  = data.timeout * 10;
	//this.timeleft = data.timeleft * 10;
	//this.username = data.username;
	this.email     = data.email;
	this.firstname = data.firstname;
	this.surname   = data.surname;

	name = " ";

	this.eSid.empty().append(this.id);
	if ('(null)' == this.firstname || '(null)' == this.surname) {
		name += this.email;
	} else {
		name += this.firstname + " " + this.surname;
	}
	$("#main p:eq(1) span:eq(0)").empty().append(name);

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
		this.interval = setInterval($.proxy(this.process, this), 100);
	}
}

Session.prototype.process = function()
{
	if (0 >= this.timeleft) {
		this.stop();
	}

	else {
		this.timeleft--;
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

	this.eSid.empty().append(this.id);
	$("#main p:eq(1) span:eq(0)").empty().append(" " + this.username);

	this.draw();
}

// vim: set ts=4 sw=4:
