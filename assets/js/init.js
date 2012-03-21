var sess = null;

$(document).ready(function() {
	var sval = new ServerVal("#randval");

	sess = new Session("#sessinfo");

	$(window).focus(function() {
		$.getJSON("/sessinfo/", $.proxy(sess.loadJSON, sess));
	});

	$("ul#menu li:eq(0)").click(function() {
		sval.start();
	});

	$("ul#menu li:eq(1)").click(function() {
		$.getJSON("/sess/", $.proxy(sess.loadJSON, sess));
	});

	$("ul#menu li:eq(2)").click(function() {
		$("#login").removeClass("hide");
	});

	$("#randval").click(function() {
		sval.stop();
	});

	$("#login form").submit(function(event) {
		event.preventDefault();
		$.post("/login/",
			$("#login form").serialize(),
			$.proxy(sess.loadJSON, sess));
		$("#login").addClass("hide");
	});
});

// vim: set ts=4 sw=4:
