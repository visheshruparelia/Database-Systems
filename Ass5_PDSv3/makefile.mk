contact_driver: contact.o pds.o bst.o contact_driver.o
	gcc -o contact_driver contact.o pds.o bst.o contact_driver.o

contact_loader: contact.o pds.o bst.o contact_loader.o
	gcc -o contact_loader contact.o pds.o bst.o contact_loader.o

pds_tester: contact.o pds.o bst.o pds_tester.o
	gcc -o pds_tester contact.o pds.o bst.o pds_tester.o

bst_tester: bst.o bstmain.o
	gcc -o bst_tester bst.o bstmain.o

pds_tester.o: pds_tester.c
	gcc -c pds_tester.c

contact_loader.o: contact_loader.c
	gcc -c contact_loader.c

contact_driver.o: contact_driver.c
	gcc -c contact_driver.c

contact.o: contact.c
	gcc -c contact.c

pds.o: pds.c
	gcc -c pds.c

bst.o: bst.c
	gcc -c bst.c

bstmain.o: bstmain.c
	gcc -c bstmain.c

clean:
	rm -f *.o
	rm -f ./contact_driver
	rm -f ./contact_loader
	rm -f ./pds_tester
	rm -f ./bst_tester

