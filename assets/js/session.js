function Session(idSelector, infoSelector, user)
{
	this.user      = user;
	this.idElement = $(idSelector);
	this.canvas    = $(infoSelector + " canvas").get(0);
	this.context   = this.canvas.getContext("2d");

	this._interval = null;

	this.update();
}

Session.prototype.update = function() {
	$.getJSON("/sessinfo/", $.proxy(this._update, this));
}

Session.prototype.clear = function() {
	this.id       = "none";
	this.timeout  = 0;
	this.timeleft = 0;

	this._fraction  = 0.0;
	this._leftWidth = 0.0;

	this.idElement.empty().append(this.id);

	this.user.update();

	this._stop();
}

/*
 * not real private but as a convention I use _ prefix to indicate
 * internal use only.
 */
Session.prototype._update = function(data)
{
	this.id        = ("0" == data.id)? "none" : data.id;
	this.timeout   = data.timeout;
	this.timeleft  = data.timeleft;

	this._fraction  = this.canvas.width / this.timeout;
	this._leftWidth = this._fraction * this.timeleft;

	this.idElement.empty().append(this.id);

	this._start();
}

Session.prototype._start = function()
{
	this._draw();
	if (0 < this.timeleft && null === this._interval) {
		this._interval = setInterval($.proxy(this._process, this), 5000);
	}
}

Session.prototype._process = function()
{
	if (0 >= this.timeleft) {
		this.clear();
	}

	else {
		this.timeleft-=5;
		this._leftWidth -= 5*this._fraction;
		this._draw();
	}
}

Session.prototype._draw = function() {
	this.context.fillStyle = "rgb(255, 0, 0)";
	this.context.fillRect(0, 0, this.canvas.width, this.canvas.height);

	this.context.fillStyle = "rgb(0, 255, 0)";
	this.context.fillRect(0, 0, this._leftWidth, this.canvas.height);
}

Session.prototype._stop = function()
{
	clearInterval(this._interval);
	this._interval = null;
}

// vim: set ts=4 sw=4:
