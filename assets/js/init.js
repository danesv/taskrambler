var sess = null;

$(document).ready(function() {
	var sval    = new ServerVal("#randval");

	sess = new Session("#sessinfo");

//	$(window).focus(function() {
//		$.getJSON("/sessinfo/", $.proxy(sess.loadJSON, sess));
//	});

	$.getJSON(
		"/version/",
		function(data) {
			$.each(result, function(i, field){
				$("#version").empty().append("version: " + field);
			});
		}
	);

	$("div#menu ul li:eq(1)").click(function() {
		sval.start();
	});

	$("div#menu ul li:eq(2)").click(function() {
		$.getJSON("/sess/", $.proxy(sess.loadJSON, sess));
	});

	$("div#menu ul li:eq(3)").click(function() {
		$("#signup").removeClass("hide");
	});

	$("div#menu ul li:eq(4)").click(function() {
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

	$("#signup form").submit(function(event) {
		event.preventDefault();
		$.post("/signup/",
			$("#signup form").serialize(),
			$.proxy(sess.loadJSON, sess));
		$("#signup").addClass("hide");
	});
});

// vim: set ts=4 sw=4:
