#include "motorController.h"

void CRectangle::set_values(int a, int b)
{
  x = a;
  y = b;
}

int CRectangle::area()
{
	return x * y;
}
