output:contact_driver.o contact.o pds.o
	gcc -o output pds.o contact.o contact_driver.o

contact_driver.o:contact.h pds.h
	gcc -c contact_driver.c
pds.o:pds.h contact.h
	gcc -c pds.c
contact.o:contact.h pds.h
	gcc -c contact.c
