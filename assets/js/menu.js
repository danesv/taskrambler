function Menu(menuSelector, application, user)
{
	this.application    = application;

	this.signupSelector = menuSelector + " ul li.signup";
	this.loginSelector  = menuSelector + " ul li.login";
	this.logoutSelector = menuSelector + " ul li.logout";

	this.menuElement   = $(menuSelector);
}

Menu.prototype.init = function() {
	this.menuElement.load("/_menu.html", $.proxy(this._menuActions, this));
}

Menu.prototype.update = function() {
	if (this.application.user.isEmpty()) {
		$(this.signupSelector).removeClass("hide");
		$(this.loginSelector).removeClass("hide");
		$(this.logoutSelector).addClass("hide");
	} else {
		$(this.signupSelector).addClass("hide");
		$(this.loginSelector).addClass("hide");
		$(this.logoutSelector).removeClass("hide");
	}
}


Menu.prototype._menuActions = function() {
	$(this.signupSelector).click(function(ev) {
		if ($("#signup-container").hasClass("hide")) {
			$("#login-container").addClass("hide");
			$("#signup-container").css("top", ev.pageY + 20);
			$("#signup-container").css("left", ev.pageX - 100);
			$("#signup-container").removeClass("hide");
		} else {
			$("#signup-container").addClass("hide");
		}
	});

	$(this.loginSelector).click(function(ev) {
		if ($("#login-container").hasClass("hide")) {
			$("#signup-container").addClass("hide");
			$("#login-container").css("top", ev.pageY + 20);
			$("#login-container").css("left", ev.pageX - 100);
			$("#login-container").removeClass("hide");
		} else {
			$("#login-container").addClass("hide");
		}
	});

	$(this.logoutSelector)
		.click($.proxy(this.application.logout, this.application));
}

// vim: set ts=4 sw=4:
