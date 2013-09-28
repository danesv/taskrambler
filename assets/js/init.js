var sess = null;

$(document).ready(function() {
	var sval    = new ServerVal("#randval");

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

	$("div#menu ul li:eq(5)").click(function() {
		$("#signup").removeClass("hide");
	});

	$("div#menu ul li:eq(6)").click(function() {
		$("#login").removeClass("hide");
	});

	$("#randval").click(function() {
		sval.stop();
	});

	$("#login form").submit(function(event) {
		event.preventDefault();
		$.post("/login/",
			$("#login form").serialize(),
			$.proxy(sess.loadUserJSON, sess));
		$("#login").addClass("hide");
	});

	$("#signup form").submit(function(event) {
		event.preventDefault();
		$.post("/signup/",
			$("#signup form").serialize(),
			$.proxy(sess.loadUserJSON, sess));
		$("#signup").addClass("hide");
	});
});

// vim: set ts=4 sw=4:
