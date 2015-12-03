#include "SpringSystem.h"



int main(int argc, char **argv) {

        simit::init(argv[2], sizeof(simit_float));
        SpringSystem t;
        if (t.loadObject(argv[1])) {
	        t.load();
    	    t.step();
    	}
    
}

