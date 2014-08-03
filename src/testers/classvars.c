#include <stdio.h>
#include <stdarg.h>

#include "trbase.h"
#include "classvars.h"

static
int
blaCtor(void * _this, va_list * params)
{
	Bla this = _this;

	this->inst_var = va_arg(*params, int);
	TR_CLASSVARS(Bla, TR_GET_CLASS(this))->class_var++;

	return 0;
}

static void blaDtor(void * _this) {};

static void blaCvInit(TR_class_ptr cls)
{
	puts("call cv init");
	TR_CLASSVARS(Bla, cls)->class_var = 0;
}

TR_INIT_IFACE(TR_Class, blaCtor, blaDtor, NULL);
TR_CREATE_CLASS(Bla, NULL, blaCvInit, TR_IF(TR_Class));

int
main(int argc, char * argv[])
{
	Bla bla1 = TR_new(Bla, 10);
	Bla bla2 = TR_new(Bla, 20);

	TR_logger = TR_new(TR_LoggerStderr);

	printf("%d\n", TR_CLASSVARS(Bla, TR_GET_CLASS(bla1))->class_var);
	printf("%d\n", TR_CLASSVARS(Bla, TR_GET_CLASS(bla2))->class_var);
	printf("%d\n", bla1->inst_var);
	printf("%d\n", bla2->inst_var);

	return 0;
}

// vim: set ts=4 sw=4:
