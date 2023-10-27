#if 0
CFLAGS="-pthread" make main && ./main && exit 0 || exit 1
#endif
#define BON_IMPL
#include "bon.h"


int main(){
	job j = MAKE_JOB("ls");
	
	BEGIN_CHAIN(X) = &j;
		CHAINS(X, "sleep 1 && echo ll");
		BEGIN_CHAIN(Y) = X;
			CHAINS(Y, "echo Y1");
			CHAINS(Y, "sleep 2");
			CHAINS(Y, "echo Y2");
		END_CHAIN(Y);
		CHAINS(X,"pwd");
	END_CHAIN(X);
	BEGIN_CHAIN(X) = &j;
		CHAINS(X, "sleep 1 && echo ll");
		BEGIN_CHAIN(Y) = X;
			CHAINS(Y, "echo Y1");
			CHAINS(Y, "sleep 2");
			for(int i = 5; i < 10;++i)
			{
				char buffer[50];
				sprintf(buffer,"seq 1 %d && echo '' ", i);
				CHAINS(Y, buffer);
			}
		END_CHAIN(Y);
		CHAINS(X,"pwd");
	END_CHAIN(X);
	return runjob(&j);
}
