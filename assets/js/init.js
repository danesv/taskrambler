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
	$("#footer").load("/_footer.html");
	$("#main").load(asset);

	$("#menu").load("/_menu.html", function() {
		$("div#menu ul li.signup").click(function() {
			$("#signup").removeClass("hide");
		});

		$("div#menu ul li.login").click(function() {
			$("#login").removeClass("hide");
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
			//$.getJSON("/authenticate/", $.proxy(sess.loadUserJSON, sess));
			$.getJSON("/sessinfo/", $.proxy(sess.loadJSON, sess));
		});

		$("#login").load("/_login.html", function (){
			$("#login form").submit(function(event) {
				event.preventDefault();
				$.post("/authenticate/",
					$("#login form").serialize(),
					$.proxy(sess.loadUserJSON, sess));
				$("#login").addClass("hide");
				$.getJSON("/sessinfo/", $.proxy(sess.loadJSON, sess));
			});
		});

		$("#signup").load("/_signup.html", function (){
			$("#signup form").submit(function(event) {
				event.preventDefault();
				$.post("/user/",
					$("#signup form").serialize(),
					$.proxy(sess.loadUserJSON, sess));
				$("#signup").addClass("hide");
				$.getJSON("/sessinfo/", $.proxy(sess.loadJSON, sess));
			});
		});
	});

	$("#randval").click(function() {
		sval.stop();
	});
});

// vim: set ts=4 sw=4:
