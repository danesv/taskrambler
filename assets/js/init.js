var sess = null;

$(document).ready(function() {
	var sval = new ServerVal("#randval");

	$("#title").load("/_title.html");
	$("#footer").load("/_footer.html");

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
		$.getJSON("/user/get/", $.proxy(sess.loadUserJSON, sess));

		$(window).focus(function() {
			$.getJSON("/sessinfo/", $.proxy(sess.loadJSON, sess));
		});

		$("div#menu ul li.logout").click(function() {
			$.getJSON("/logout/", $.proxy(sess.loadUserJSON, sess));
			$.getJSON("/sessinfo/", $.proxy(sess.loadJSON, sess));
		});

		$("#login").load("/_login.html", function (){
			$("#login form").submit(function(event) {
				event.preventDefault();
				$.post("/login/",
					$("#login form").serialize(),
					$.proxy(sess.loadUserJSON, sess));
				$("#login").addClass("hide");
				$.getJSON("/sessinfo/", $.proxy(sess.loadJSON, sess));
			});
		});

		$("#signup").load("/_signup.html", function (){
			$("#signup form").submit(function(event) {
				event.preventDefault();
				$.post("/signup/",
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
