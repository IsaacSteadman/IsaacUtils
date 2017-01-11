#include "Test1IsaacUtils.h"

void InitTests() {
	Utils::Init();
	AddTestCase(new Test_wString());
	AddTestCase(new Test_String());
	AddTestCase(new Test_BaseBigLong());
	AddTestCase(new Test_BigLong());
}
void DeInitTests() {
	Utils::DeInit();
}
