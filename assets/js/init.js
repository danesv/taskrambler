var sess = null;

$(document).ready(function() {
	var sval        = new ServerVal("#randval");
	var asset_exp   = /^.*\/(.*)/;
	var asset       = '/_main.html';
	var application = new Application();
	var menu        = new Menu("#menu", application);

	var title_adds = {
		'/_author.html'        : ' - Author',
		'/_documentation.html' : ' - Documentation',
		'/_download.html'      : ' - Download',
	};

	asset_exp.exec(this.location);

	if ('' != RegExp.$1) {
		asset = '/_' + RegExp.$1;
	}

	if (asset in title_adds) {
		this.title += title_adds[asset];
	}

	$("#title").load("/_title.html");
	$("#main").load(asset, function() {
		$("#main").append('<img id="gplv3" src="image/gplv3-127x51.png" />')
	});
	menu.init(application);

	$("#statusline").load("/_statusline.html", function() {
		application.init(menu);
	});

	$("#login").load("/_login.html", function() {
		$(function() {
			$("#login-container").draggable();
		});

		$("#login-close").click(function() {
			$("#login-container").addClass("hide");
		});

		$("#login form").submit($.proxy(application.login, application));
	});

	$("#signup").load("/_signup.html", function (){
		$(function() {
			$("#signup-container").draggable();
		});

		$("#signup-close").click(function (e) {
			$("#signup-container").addClass("hide");
		});

		$("#signup form").submit($.proxy(application.signup, application));
	});

	$("#myaccount").load("/_myaccount.html", function (){
		$(function() {
			$("#myaccount-container").draggable();
		});

		$("#myaccount-close").click(function (e) {
			$("#myaccount-container").addClass("hide");
		});

		$("#myaccount form").submit(
			$.proxy(application.userupdate, application));
	});

	$("#footer").load("/_footer.html", function (){
		$.getJSON(
			"/loc/",
			function(data) {
				$("#loc").empty().append(data.loc + " lines of C code");
			}
		);
	});

	$("#randval").click(function() {
		sval.stop();
	});

});

// vim: set ts=4 sw=4:
