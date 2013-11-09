var sess = null;

$(document).ready(function() {
	var sval      = new ServerVal("#randval");
	var asset_exp = /^.*\/(.*)/;
	var asset     = '/_main.html';

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
	$("#main").load(asset);

	$("#menu").load("/_menu.html", function() {
		$("div#menu ul li.signup").click(function(e) {
			if ($("#signup-container").hasClass("hide")) {
				$("#login-container").addClass("hide");
				$("#signup-container").css("top", e.pageY + 20);
				$("#signup-container").css("left", e.pageX - 100);
				$("#signup-container").removeClass("hide");
			} else {
				$("#signup-container").addClass("hide");
			}
		});

		$("div#menu ul li.login").click(function(e) {
			if ($("#login-container").hasClass("hide")) {
				$("#signup-container").addClass("hide");
				$("#login-container").css("top", e.pageY + 20);
				$("#login-container").css("left", e.pageX - 100);
				$("#login-container").removeClass("hide");
			} else {
				$("#login-container").addClass("hide");
			}
		});
	});

	$("#statusline").load("/_statusline.html", function (){
		sess = new Session("#sessinfo", "#sessid", "#user");

		$.getJSON(
			"/version/",
			function(data) {
				$("#version").empty().append("version: " + data.version);
			}
		);

		$.getJSON("/sessinfo/", $.proxy(sess.loadJSON, sess));
		$.getJSON("/currentuser/", $.proxy(sess.loadUserJSON, sess));

		$(window).focus(function() {
			$.getJSON("/sessinfo/", $.proxy(sess.loadJSON, sess));
		});

		$("div#menu ul li.logout").click(function() {
			$.ajax({
				dataType: "json",
				url: "/authenticate/",
				type: 'DELETE',
				success: $.proxy(sess.loadUserJSON, sess)
			});
			$.getJSON("/sessinfo/", $.proxy(sess.loadJSON, sess));
		});

		$("#login").load("/_login.html", function (){
			$(function() {
				$("#login-container").draggable();
			});

			$("#login-close").click(function (e) {
				$("#login-container").addClass("hide");
			});

			$("#login form").submit(function(event) {
				event.preventDefault();
				$.post("/authenticate/",
					$("#login form").serialize(),
					$.proxy(sess.loadUserJSON, sess));
				$("#login-container").addClass("hide");
				$.getJSON("/sessinfo/", $.proxy(sess.loadJSON, sess));
			});
		});

		$("#signup").load("/_signup.html", function (){
			$(function() {
				$("#signup-container").draggable();
			});

			$("#signup-close").click(function (e) {
				$("#signup-container").addClass("hide");
			});

			$("#signup form").submit(function(event) {
				event.preventDefault();
				$.post("/signup/",
					$("#signup form").serialize(),
					$.proxy(sess.loadUserJSON, sess));
				$("#signup-container").addClass("hide");
				$.getJSON("/sessinfo/", $.proxy(sess.loadJSON, sess));
			});
		});
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
