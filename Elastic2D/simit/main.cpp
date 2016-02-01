#include "Elastic2D.h"

int main(int argc, char **argv) {

        simit::init(argv[argc-1], sizeof(simit_float));
        Elastic2D t;
        if ((argc > 2) && (t.loadObject(argv[1]))) {
	        t.load();
    	    t.step();
    	} else {
			t.load();
			t.step();
		}
    
}

